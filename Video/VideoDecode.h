#ifndef VIDEODECODE_H
#define VIDEODECODE_H

#include <QObject>
#include <QThread>


class AVPacket;
class AVFrame;
class AVCodecContext;
class AVFormatContext;
class SwsContext;

namespace clz {

class VideoDecode : public QThread
{
    Q_OBJECT
public:
    VideoDecode(QObject* parent = 0);
    void run() override;

    void set_video_file(const QString& path);

    void ffmpeg_init();
    // 中途更换播放源
    void reset();

    void close();

signals:
    void sig_video_info_decoded(uchar* data, int w, int h);

private:
    QString m_video_path;

    AVCodecContext* m_avcodec_context;
    AVFormatContext* m_avformat_context;
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

#endif // VIDEODECODE_H
