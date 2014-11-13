#ifndef VIDEOFILTER_H
#define VIDEOFILTER_H

#include <QObject>
#include <QProgressDialog>

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
    explicit VideoFilter(QObject *parent = 0);
    ~VideoFilter ();

    enum RotateDirection {
        ClockWiseFlip,
        ClockWise,
        CounterClockWiseFlip,
        CounterClockWise
    };


signals:
    void rotate (QString fileName, VideoFilter::RotateDirection direction);

    void progressChanged (double percent);
    void rotateDone ();

public slots:
    void transposeAsync (QString fileName, VideoFilter::RotateDirection direction);

private slots:

private:
    AVFormatContext *_inCtx;
    AVFormatContext *_outCtx;
    FilteringContext *_filterCtx;

    QProgressDialog _operationDialog;

    void initAV();

    bool openInputFile(QString fileName);
    bool openOutputTempFile(QString fileName);
    bool replaceInputFile(QString fileName);
    QString getTempFileName (QString fileName);

    bool initFilters (RotateDirection direction);
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


