#include "videofilter.h"

#include <QDir>
#include <QDebug>
#include <QDate>
#include <QSignalMapper>

// ffmpeg -i 0001_00001.MTS -vf "transpose=1" -vcodec libx264 -acodec copy 0001_00001_ffmpeg.mts

inline int snprintf(char* str, size_t size, const char* format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = vsnprintf(str, size, format, ap);
    va_end(ap);

    return count;
}

VideoFilter::VideoFilter(QObject *parent) :
    QObject(parent)
{
    _filterCtx = NULL;

    _operationDialog.setLabelText(tr("Rotating video"));
    _operationDialog.setCancelButtonText(tr("Cancel"));
    _operationDialog.setMinimum(0);
    _operationDialog.setMaximum(100);
    _operationDialog.setWindowModality(Qt::ApplicationModal);
    _operationDialog.hide();
}

VideoFilter::~VideoFilter()
{
}


void VideoFilter::transposeAsync(QString fileName, VideoFilter::RotateDirection direction)
{
    _operationDialog.show();

    initAV ();

    // showFFMpegInfo();

    openInputFile (fileName);
    openOutputTempFile (fileName);

    initFilters (direction);

    copyMetadata();

    AVPacket packet;

    while (readPacket(&packet) >= 0) {
        updateProgress (&packet);

        writePacket(&packet);

        av_free_packet(&packet);
    }

    flush ();
    close ();

    replaceInputFile (fileName);

    emit rotateDone ();

    _operationDialog.hide();
}

void VideoFilter::updateProgress (AVPacket *pkt)
{
    int streamIndex;
    AVStream *stream;

    streamIndex = pkt->stream_index;
    stream = _inCtx->streams[streamIndex];
    if (stream->codec->codec_type == AVMEDIA_TYPE_VIDEO && pkt->dts > 0) {
        int total;

        total = stream->duration + stream->start_time;

        // qDebug () << "Progress: " << ((double) (pkt->dts * 100) / (double)total) << "%";

        _operationDialog.setValue((pkt->dts * 100) / total);

        emit progressChanged((pkt->dts * 100) / total);
    }
}

void VideoFilter::copyMetadata()
{
    av_dict_copy (&_outCtx->metadata, _inCtx->metadata, 0);
}

void VideoFilter::initAV()
{
    av_register_all();
    avfilter_register_all();
    avcodec_register_all();
}

bool VideoFilter::openInputFile(QString fileName)
{
    QByteArray fileNameChar;
    _inCtx = NULL;

    fileNameChar = fileName.toLatin1();
    if (avformat_open_input(&_inCtx, fileNameChar.data(), NULL, NULL) < 0) {
        return false;
    }

    if (avformat_find_stream_info(_inCtx, NULL) < 0) {
        return false;
    }

    for (unsigned int i = 0; i < _inCtx->nb_streams; i++) {
        AVStream *stream;
        AVCodecContext *codecCtx;

        stream = _inCtx->streams[i];
        codecCtx = stream->codec;

        if (codecCtx->codec_type == AVMEDIA_TYPE_VIDEO ||
            codecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (avcodec_open2(codecCtx, avcodec_find_decoder(codecCtx->codec_id), NULL) < 0) {
                return false;
            }
        }
    }

#ifdef QT_DEBUG
    av_dump_format(_inCtx, 0, fileNameChar.data(), 0);
#endif

    return true;
}

bool VideoFilter::openOutputTempFile(QString fileName)
{
    AVStream *outStream;
    AVStream *inStream;
    AVCodecContext *decCtx;
    AVCodecContext *encCtx;
    AVCodec *encoder;
    unsigned int i;
    QByteArray tempFileName;

    tempFileName = getTempFileName(fileName).toLatin1();

    qDebug () << "Out file: " << getTempFileName(fileName);

    _outCtx = NULL;
    if (avformat_alloc_output_context2(&_outCtx, NULL, NULL, tempFileName.data()) < 0) {
        return false;
    }

    for (i = 0; i < _inCtx->nb_streams; i++) {
        if (!(_inCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO ||
              _inCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)) {
            continue;
        }

        outStream = avformat_new_stream(_outCtx, NULL);
        if (!outStream) {
            return false;
        }

        inStream = _inCtx->streams[i];
        decCtx = inStream->codec;
        encCtx = outStream->codec;

        if (decCtx->codec_type == AVMEDIA_TYPE_VIDEO ||
                decCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
            // Encode to the same codec and the same properties
            encoder = avcodec_find_encoder(decCtx->codec_id);
            if (!encoder) {
                qDebug () << "avcodec_find_encoder returns NULL: " << "-.-" << decCtx->codec_descriptor->long_name << ": " << decCtx->codec_id;
                return false;
            }

            if (decCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
                // In the rotation change the width and the height
                encCtx->height = decCtx->width;
                encCtx->width = decCtx->height;

                encCtx->pix_fmt = encoder->pix_fmts[0];
                encCtx->time_base = decCtx->time_base;

                encCtx->bit_rate = decCtx->bit_rate;
                encCtx->global_quality = decCtx->global_quality;
                encCtx->bit_rate_tolerance = decCtx->bit_rate_tolerance;
                encCtx->sample_aspect_ratio = decCtx->sample_aspect_ratio;

                // ffmpeg mandatory
                encCtx->qmin = 10;
                encCtx->qmax = 51;
                encCtx->max_qdiff = 4;
            }
            else if (decCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
                encCtx->sample_rate = decCtx->sample_rate;
                encCtx->channel_layout = decCtx->channel_layout;
                encCtx->channels = av_get_channel_layout_nb_channels(encCtx->channel_layout);

                // take first format from list of supported formats
                encCtx->sample_fmt = encoder->sample_fmts[0];

                outStream->time_base = inStream->time_base;
            }
        }
        else if (decCtx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
            continue;
        }
        else if (decCtx->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            continue;
            /*
            encoder = avcodec_find_encoder(AV_CODEC_ID_DVD_SUBTITLE);
            avcodec_get_context_defaults3(encCtx, encoder);
            encCtx->width = decCtx->with;
            encCtx->height = decCtx->height;
            */
        }
        else {
            // if this stream must be remuxed
            if (avcodec_copy_context(encCtx, decCtx) < 0) {
                return false;
            }
        }

        if (encoder) {
            if (avcodec_open2(encCtx, encoder, NULL) < 0) {
                return false;
            }
        }
        else {
            return false;
        }

        if (_outCtx->oformat->flags & AVFMT_GLOBALHEADER) {
            encCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;
        }
    }

#ifdef QT_DEBUG
    av_dump_format(_outCtx, 0, tempFileName.data(), 1);
#endif

    if (!(_outCtx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&_outCtx->pb, tempFileName.data(), AVIO_FLAG_WRITE) < 0) {
            return false;
        }
    }

    // init muxer, write output file header
    if (avformat_write_header(_outCtx, NULL) < 0) {
        return false;
    }

    return true;
}

QString VideoFilter::getTempFileName(QString fileName)
{
    QFileInfo info(fileName);

    // return QDir::tempPath().append("/").append("_").append(info.fileName());
    return info.absolutePath().append("/_").append(info.fileName());
}


bool VideoFilter::initFilters(VideoFilter::RotateDirection direction)
{
    const char *filterSpec;

    _filterCtx = new FilteringContext [_inCtx->nb_streams];

    if (!_filterCtx) {
        return false;
    }

    for (unsigned int i = 0; i < _inCtx->nb_streams; i++) {
        // Only filter audio and video streams
        if (!(_inCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO ||
              _inCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)) {
            continue;
        }

        if (_inCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            switch (direction) {
            case RotateDirection::ClockWise:
                filterSpec = "transpose=clock";
                break;
            case RotateDirection::ClockWiseFlip:
                filterSpec = "transpose=clock_flip";
                break;
            case RotateDirection::CounterClockWise:
                filterSpec = "transpose=cclock";
                break;
            case RotateDirection::CounterClockWiseFlip:
                filterSpec = "transpose=cclock_flip";
                break;
            }
        }
        else if (_inCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            // passthrough (dummy) filter for audio
            filterSpec = "anull";
            // filterSpec = "aac_adtstoasc";
        }

        if (!initFilter(&_filterCtx[i],
                        _inCtx->streams[i]->codec,
                        _outCtx->streams[i]->codec,
                        filterSpec)) {
            return false;
        }
    }

    return true;
}

bool VideoFilter::initFilter(FilteringContext* fctx,
                             AVCodecContext *decCtx,
                             AVCodecContext *encCtx,
                             const char *filterSpec)
{
    AVFilterContext *bufferCtxIn = NULL;
    AVFilterContext *bufferCtxOut = NULL;
    AVFilterInOut *outputs = NULL;
    AVFilterInOut *inputs = NULL;
    AVFilterGraph *filterGraph = NULL;

    filterGraph = avfilter_graph_alloc();
    if (!filterGraph) {
        return false;
    }

    outputs = avfilter_inout_alloc();
    inputs = avfilter_inout_alloc();

    if (outputs && inputs) {
        if (decCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
            bufferCtxIn = createVideoBufferContextIn (decCtx, filterGraph);
            bufferCtxOut = createVideoBufferContextOut (encCtx, filterGraph);
        }
        else if (decCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
            bufferCtxIn = createAudioBufferContextIn (decCtx, filterGraph);
            bufferCtxOut = createAudioBufferContextOut (encCtx, filterGraph);
        }

        if (bufferCtxIn && bufferCtxOut) {
            // Endpoints for the filter graph.
            outputs->name       = av_strdup("in");
            outputs->filter_ctx = bufferCtxIn;
            outputs->pad_idx    = 0;
            outputs->next       = NULL;

            inputs->name       = av_strdup("out");
            inputs->filter_ctx = bufferCtxOut;
            inputs->pad_idx    = 0;
            inputs->next       = NULL;

            if (avfilter_graph_parse_ptr(filterGraph,
                                     filterSpec,
                                     &inputs,
                                     &outputs, NULL) != -1) {
                avfilter_graph_config(filterGraph, NULL);

                /* Fill FilteringContext */
                fctx->setBufferSrcCtx(bufferCtxIn);
                fctx->setBufferSinkCtx(bufferCtxOut);
                fctx->setFilterGraph(filterGraph);
            }
        }
    }

    if (inputs) {
        avfilter_inout_free(&inputs);
    }

    if (outputs) {
        avfilter_inout_free(&outputs);
    }

    return true;
}

AVFilterContext *VideoFilter::createVideoBufferContextIn(AVCodecContext *ctx, AVFilterGraph *filterGraph)
{
    AVFilter *buffer = NULL;
    AVFilterContext *result = NULL;
    char args[512];

    buffer = avfilter_get_by_name("buffer");
    if (buffer) {
        snprintf(args,
                 sizeof(args),
                 "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
                 ctx->width,
                 ctx->height,
                 ctx->pix_fmt,
                 ctx->time_base.num,
                 ctx->time_base.den,
                 ctx->sample_aspect_ratio.num,
                 ctx->sample_aspect_ratio.den);

        avfilter_graph_create_filter(&result,
                                     buffer,
                                     "in",
                                     args,
                                     NULL,
                                     filterGraph);
    }

    return result;
}

AVFilterContext *VideoFilter::createVideoBufferContextOut(AVCodecContext *ctx, AVFilterGraph *filterGraph)
{
    AVFilter *buffer = NULL;
    AVFilterContext *result = NULL;

    buffer = avfilter_get_by_name("buffersink");
    if (buffer) {
        avfilter_graph_create_filter(&result,
                                     buffer,
                                     "out",
                                     NULL,
                                     NULL,
                                     filterGraph);
        if (result) {
            av_opt_set_bin(result,
                           "pix_fmts",
                           (uint8_t*)&ctx->pix_fmt,
                           sizeof(ctx->pix_fmt),
                           AV_OPT_SEARCH_CHILDREN);
        }
    }

    return result;
}

AVFilterContext *VideoFilter::createAudioBufferContextIn(AVCodecContext *ctx, AVFilterGraph *filterGraph)
{
    char args[512];
    AVFilter *buffer = NULL;
    AVFilterContext *result = NULL;

    buffer = avfilter_get_by_name("abuffer");
    if (buffer) {
        if (!ctx->channel_layout) {
            ctx->channel_layout = av_get_default_channel_layout(ctx->channels);
        }

        snprintf(args,
                 sizeof(args),
                 "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=0x%" PRIx64,
                 ctx->time_base.num,
                 ctx->time_base.den,
                 ctx->sample_rate,
                 av_get_sample_fmt_name(ctx->sample_fmt),
                 ctx->channel_layout);

        avfilter_graph_create_filter(&result,
                                     buffer,
                                     "in",
                                     args,
                                     NULL,
                                     filterGraph);
    }

    return result;
}

AVFilterContext *VideoFilter::createAudioBufferContextOut(AVCodecContext *ctx, AVFilterGraph *filterGraph)
{
    AVFilter *buffer = NULL;
    AVFilterContext *result = NULL;

    buffer = avfilter_get_by_name("abuffersink");
    if (buffer) {
        avfilter_graph_create_filter(&result,
                                     buffer,
                                     "out",
                                     NULL,
                                     NULL,
                                     filterGraph);

        if (buffer) {
            av_opt_set_bin(result,
                           "sample_fmts",
                           (uint8_t*)&ctx->sample_fmt,
                           sizeof(ctx->sample_fmt),
                           AV_OPT_SEARCH_CHILDREN);

            av_opt_set_bin(result,
                           "channel_layouts",
                           (uint8_t*)&ctx->channel_layout,
                           sizeof(ctx->channel_layout),
                           AV_OPT_SEARCH_CHILDREN);

            av_opt_set_bin(result,
                           "sample_rates",
                           (uint8_t*)&ctx->sample_rate,
                           sizeof(ctx->sample_rate),
                           AV_OPT_SEARCH_CHILDREN);
        }
    }

    return result;
}

int VideoFilter::readPacket(AVPacket *packet)
{
    packet->data = NULL;
    packet->size = 0;

    return av_read_frame(_inCtx, packet);
}

void VideoFilter::writePacket(AVPacket *packet)
{
    int streamIndex;
    enum AVMediaType type;

    streamIndex = packet->stream_index;
    type = _inCtx->streams[streamIndex]->codec->codec_type;

    if (type == AVMEDIA_TYPE_VIDEO || type == AVMEDIA_TYPE_AUDIO) {
        // qDebug() << "Demuxer gave frame of stream_index " << streamIndex;

        if (_filterCtx[streamIndex].filterGraph()) {
            AVFrame *frame;

            // qDebug() << "Going to reencode&filter the frame";

            frame = av_frame_alloc();
            if (frame) {
                av_packet_rescale_ts(packet,
                                     _inCtx->streams[streamIndex]->time_base,
                                     _inCtx->streams[streamIndex]->codec->time_base);

                int gotFrame;
                int ret;

                gotFrame = 0;
                ret = -1;
                if (type == AVMEDIA_TYPE_VIDEO) {
                    ret = avcodec_decode_video2(_inCtx->streams[streamIndex]->codec,
                                                frame,
                                                &gotFrame,
                                                packet);
                }
                else if (type == AVMEDIA_TYPE_AUDIO) {
                    ret = avcodec_decode_audio4(_inCtx->streams[streamIndex]->codec,
                                                frame,
                                                &gotFrame,
                                                packet);
                }

                if (ret < 0) {
                    av_frame_free(&frame);

                    // qDebug () <<  "Decoding failed";
                }

                if (gotFrame) {
                    frame->pts = av_frame_get_best_effort_timestamp(frame);

                    filterEncodeWriteFrame(frame, streamIndex);
                }

                av_frame_free(&frame);
            }
        }
        else {
            // remux this frame without reencoding
            av_packet_rescale_ts(packet,
                                 _inCtx->streams[streamIndex]->time_base,
                                 _outCtx->streams[streamIndex]->time_base);

            av_interleaved_write_frame(_outCtx, packet);
        }
    }
/*
    else if (type == AVMEDIA_TYPE_SUBTITLE) {
        AVSubtitle subtitle;
        int result;
        int gotSubtitle;

        result = avcodec_decode_subtitle2(_inCtx->streams[streamIndex]->codec,
                                          &subtitle,
                                          &gotSubtitle,
                                          &packet);
        if(result >= 0) {
            AVPacket packetOut;
            uint64_t bufferSize = 1024 * 1024;
            uint8_t *buffer;
            int size;

            buffer = new uint8_t[bufferSize];
            memset(buffer, 0, bufferSize);

            size = avcodec_encode_subtitle(_outCtx->streams[streamIndex]->codec,
                                    buffer,
                                    bufferSize,
                                    &subtitle);
            if (size >= 0) {
                av_init_packet(&packetOut);
                packetOut.data = buffer;
                packetOut.size = size;
                packetOut.pts = packet.pts;
                packetOut.duration = packet.duration;
                packetOut.dts = packetOut.pts;

                packet.data = buffer;
                packet.size = size;

                av_interleaved_write_frame(_outCtx, &packet);
            }
        }
    }
*/
}

int VideoFilter::filterEncodeWriteFrame(AVFrame *frame, unsigned int streamIndex)
{
    int ret;
    AVFrame *filtFrame;

    qDebug () << "Pushing decoded frame to filters";

    // push the decoded frame into the filtergraph
    ret = av_buffersrc_add_frame_flags(_filterCtx[streamIndex].bufferSrcCtx(),
                                       frame,
                                       0);
    if (ret < 0) {
        return ret;
    }

    // pull filtered frames from the filtergraph
    while (1) {
        filtFrame = av_frame_alloc();
        if (!filtFrame) {
            ret = AVERROR(ENOMEM);
            break;
        }

        qDebug () << "Pulling filtered frame from filters";
        ret = av_buffersink_get_frame(_filterCtx[streamIndex].bufferSinkCtx(),
                                      filtFrame);
        if (ret < 0) {
            qDebug () << "Pulling filtered frame: ret < 0: " << ret << "-.-" << (ret == AVERROR_EOF);

            /* if no more frames for output - returns AVERROR(EAGAIN)
             * if flushed and no more frames for output - returns AVERROR_EOF
             * rewrite retcode to 0 to show it as normal procedure completion
             */
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                ret = 0;

            av_frame_free(&filtFrame);
            break;
        }

        filtFrame->pict_type = AV_PICTURE_TYPE_NONE;

        qDebug () << "Call encodeWriteFrame";
        ret = encodeWriteFrame(filtFrame, streamIndex, NULL);
        if (ret < 0)
            break;
    }

    return ret;
}

int VideoFilter::encodeWriteFrame(AVFrame *filtFrame, unsigned int streamIndex, int *gotFrame)
{
    int ret;
    int gotFrameLocal;
    AVPacket encPkt;

    int (*enc_func)(AVCodecContext *, AVPacket *, const AVFrame *, int *) =
        (_inCtx->streams[streamIndex]->codec->codec_type == AVMEDIA_TYPE_VIDEO) ? avcodec_encode_video2 : avcodec_encode_audio2;

    if (!gotFrame)
        gotFrame = &gotFrameLocal;

    qDebug () << "Encoding frame!!";

    // encode filtered frame
    encPkt.data = NULL;
    encPkt.size = 0;
    av_init_packet(&encPkt);

    ret = enc_func(_outCtx->streams[streamIndex]->codec,
                   &encPkt,
                   filtFrame,
                   gotFrame);

    av_frame_free(&filtFrame);

    if (ret < 0) {
        return ret;
    }

    if (!(*gotFrame)) {
        return 0;
    }

    // prepare packet for muxing
    encPkt.stream_index = streamIndex;
    av_packet_rescale_ts(&encPkt,
                         _outCtx->streams[streamIndex]->codec->time_base,
                         _outCtx->streams[streamIndex]->time_base);

    if (useADTSToASCFilter (streamIndex)) {
        AVBitStreamFilterContext *bscf;
        AVPacket newpacket = encPkt;
        int rc;

        bscf = av_bitstream_filter_init("aac_adtstoasc");
        rc = av_bitstream_filter_filter(bscf,
                                   _outCtx->streams[streamIndex]->codec,
                                   NULL,
                                   &newpacket.data,
                                   &newpacket.size,
                                   encPkt.data,
                                   encPkt.size,
                                   encPkt.flags & AV_PKT_FLAG_KEY);

        if (rc < 0) {
            ret = av_interleaved_write_frame(_outCtx, &encPkt);
        }
        else {
            ret = av_interleaved_write_frame(_outCtx, &newpacket);
        }
    }
    else {
        ret = av_interleaved_write_frame(_outCtx, &encPkt);
    }

    av_free_packet(&encPkt);

    qDebug () << "encodeWriteFrame done: " << ret;

    return ret;
}

bool VideoFilter::useADTSToASCFilter (int streamIndex)
{
    AVCodecContext *ctx;

    ctx = _inCtx->streams[streamIndex]->codec;

    return ctx->codec_type == AVMEDIA_TYPE_AUDIO &&
            ctx->codec_id == AV_CODEC_ID_AAC;
}

void VideoFilter::flush()
{
    // flush filters and encoders
    for (unsigned int i = 0; i < _inCtx->nb_streams; i++) {
        if (!(_inCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO ||
              _inCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)) {
            continue;
        }

        qDebug () << "Flushing-........";

        // flush filter
        if (!_filterCtx[i].filterGraph())
            continue;

        qDebug () << "Flushing 2-........";

        filterEncodeWriteFrame(NULL, i);

        // flush encoder
        flushEncoder(i);
    }

    av_write_trailer(_outCtx);
}

int VideoFilter::flushEncoder (int streamIndex)
{
    int ret;
    int gotFrame;

    qDebug () << "Flusing encoder " << streamIndex;

    if (_outCtx->streams[streamIndex]->codec->codec->capabilities & CODEC_CAP_DELAY) {
        while (1) {
            qDebug () << "Encode and write frame ";

            ret = encodeWriteFrame(NULL, streamIndex, &gotFrame);

            if (ret < 0)
                break;

            if (!gotFrame)
                return 0;
        }

        return ret;
    }

    return 1;
}

void VideoFilter::close()
{
    for (unsigned int i = 0; i < _inCtx->nb_streams; i++) {
        if (!(_inCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO ||
              _inCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)) {
            continue;
        }

        avcodec_close(_inCtx->streams[i]->codec);
        if (_outCtx &&
                _outCtx->nb_streams > i &&
                _outCtx->streams[i] &&
                _outCtx->streams[i]->codec) {
            avcodec_close(_outCtx->streams[i]->codec);
        }
    }

    delete [] _filterCtx;

    avformat_close_input(&_inCtx);

    if (_outCtx && !(_outCtx->oformat->flags & AVFMT_NOFILE))
        avio_close(_outCtx->pb);

    avformat_free_context(_outCtx);
}


bool VideoFilter::replaceInputFile(QString fileName)
{
    QFileInfo info;
    QString rotatedPath;

    info.setFile(fileName);
    rotatedPath = info.absolutePath().append("/").append(".rotated_video");

    if (info.absoluteDir().mkpath(rotatedPath)) {
        QString newFileName;

        newFileName = rotatedPath;
        newFileName.append("/");
        newFileName.append(info.fileName());
        newFileName.append(".");
        newFileName.append(QDateTime::currentDateTime().toString ("yyyyMMddhhmmss"));

        if (QFile::rename(fileName, newFileName)) {
            qDebug () << "New file name: " << newFileName;
            if (QFile::rename(getTempFileName(fileName), fileName)) {
                qDebug () << "Old file overwrited";

                return true;
            }
            else {
                qDebug () << "Can't rename temp file to file:" << getTempFileName(fileName) << "-.-" << fileName;
            }
        }
        else {
            qDebug () << "Can't rename file to new file: " << newFileName;
        }
    }

    return false;
}

void VideoFilter::showFFMpegInfo()
{
    AVCodec * codec = av_codec_next(NULL);
    qDebug () << "Codecs:";
    qDebug () << "********************************************";
    while(codec != NULL)
    {
        qDebug () << codec->long_name;

        codec = av_codec_next(codec);
    }

    AVOutputFormat * oformat = av_oformat_next(NULL);
    qDebug () << "\n\n";
    qDebug () << "Formats:";
    qDebug () << "********************************************";
    while(oformat != NULL)
    {
        qDebug () << oformat->long_name;

        oformat = av_oformat_next(oformat);
    }

    qDebug () << "\n\n";
    qDebug () << "Recomended codecs for formats:";
    qDebug () << "********************************************";
    oformat = av_oformat_next(NULL);
    while(oformat != NULL)
    {
        qDebug () << oformat->long_name;
        if (oformat->codec_tag != NULL)
        {
            int i = 0;

            AVCodecID cid = CODEC_ID_MPEG1VIDEO;
            while (cid != CODEC_ID_NONE)
            {
                cid = av_codec_get_id(oformat->codec_tag, i++);
                qDebug () << "     " << cid;
            }
        }

        oformat = av_oformat_next(oformat);
    }

    qDebug () << "********************************************";
}

/**************************************************************************
 * FilteringContext class
 **************************************************************************/

FilteringContext::FilteringContext()
{
    _bufferSinkCtx = NULL;
    _bufferSrcCtx = NULL;
    _filterGraph = NULL;
}

FilteringContext::~FilteringContext()
{
    if (_filterGraph) {
        // Free the links inside (_bufferSincCtx and _bufferSrcCtx)
        avfilter_graph_free (&_filterGraph);
        _filterGraph = NULL;
    }
}

void FilteringContext::setBufferSinkCtx(AVFilterContext *value)
{
    _bufferSinkCtx = value;
}

void FilteringContext::setBufferSrcCtx(AVFilterContext *value)
{
    _bufferSrcCtx = value;
}

void FilteringContext::setFilterGraph(AVFilterGraph *value)
{
    _filterGraph = value;
}


