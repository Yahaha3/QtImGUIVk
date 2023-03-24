#ifndef VIDEOSTREAMPUSH_H
#define VIDEOSTREAMPUSH_H

#include <QObject>
#include <QThread>

class AVPacket;
class AVFrame;
class AVCodecContext;
class AVFormatContext;
class SwsContext;

namespace clz {
class VideoStreamPush: public QThread
{
    Q_OBJECT
public:
    VideoStreamPush(QObject* parent = 0);
    void run() override;
    void close();

    void ffmpeg_init();

    void set_src_path(const QString& path);
    void set_des_path(const QString& path);
    void set_protocol(const QString& protocol);

private:
    QString m_src;
    QString m_des;
    QString m_protocol;

    AVCodecContext* m_avcodec_context;
    AVFormatContext* m_avformat_context_input;
    AVFormatContext* m_avformat_context_output;
    SwsContext* m_sws_context;
    // 帧缓存数据
    AVPacket* m_av_packet;
    // 帧输入
    AVFrame* m_frame_in;
    // 帧输出
    AVFrame* m_frame_rgb;
    // 缓冲区
    uint8_t* m_outbuffer;

};
}

#endif // VIDEOSTREAMPUSH_H
