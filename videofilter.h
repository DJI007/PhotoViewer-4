#ifndef VIDEOFILTER_H
#define VIDEOFILTER_H

#include <QObject>

extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/avcodec.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>

}

class FilteringContext
{
public:
    explicit FilteringContext ();
    ~FilteringContext ();

    AVFilterContext *bufferSinkCtx () { return _bufferSinkCtx; }
    AVFilterContext *bufferSrcCtx () { return _bufferSrcCtx; }
    AVFilterGraph *filterGraph () { return _filterGraph; }

    void setBufferSinkCtx (AVFilterContext *value);
    void setBufferSrcCtx (AVFilterContext *value);
    void setFilterGraph (AVFilterGraph *value);

private:
    AVFilterContext *_bufferSinkCtx;
    AVFilterContext *_bufferSrcCtx;
    AVFilterGraph *_filterGraph;
};

class VideoFilter : public QObject
{
    Q_OBJECT
public:
    explicit VideoFilter(QString fileName, QObject *parent = 0);
    ~VideoFilter ();

    enum TransposeDirection {
        ClockWiseFlip,
        ClockWise,
        CounterClockWiseFlip,
        CounterClockWise
    };

signals:
    void progressChanged (double percent);
    void transposeDone ();

public slots:
    void transpose (VideoFilter::TransposeDirection direction);

private:
    QString _fileName;
    AVFormatContext *_inCtx;
    AVFormatContext *_outCtx;
    FilteringContext *_filterCtx;

    void initAV();

    bool openInputFile();
    bool openOutputTempFile();
    bool replaceInputFile();
    QString getTempFileName ();

    bool initFilters (TransposeDirection direction);
    bool initFilter (FilteringContext *fctx,
                     AVCodecContext *decCtx,
                     AVCodecContext *encCtx,
                     const char *filterSpec);

    void copyMetadata ();

    bool useADTSToASCFilter (int streamIndex);

    void updateProgress (AVPacket *pkt);

    AVFilterContext *createVideoBufferContextIn (AVCodecContext *ctx, AVFilterGraph *filterGraph);
    AVFilterContext *createVideoBufferContextOut (AVCodecContext *ctx, AVFilterGraph *filterGraph);
    AVFilterContext *createAudioBufferContextIn (AVCodecContext *ctx, AVFilterGraph *filterGraph);
    AVFilterContext *createAudioBufferContextOut (AVCodecContext *ctx, AVFilterGraph *filterGraph);

    int readPacket(AVPacket *packet);
    void writePacket (AVPacket *packet);

    int filterEncodeWriteFrame(AVFrame *frame, unsigned int streamIndex);
    int encodeWriteFrame(AVFrame *filtFrame, unsigned int streamIndex, int *gotFrame);

    void flush ();
    int flushEncoder (int streamIndex);
    void close ();

    void showFFMpegInfo ();
};

#endif // VIDEOFILTER_H


