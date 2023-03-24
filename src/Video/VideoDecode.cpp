#include "VideoDecode.h"
#include <iostream>
#include <QImage>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
    #include <libavutil/avutil.h>
    #include <libavutil/imgutils.h>
}

void check_ffmpeg_error(int result, size_t size){
    char* error_info = new char[32];
    av_strerror(result, error_info, size);
    std::cout << "open error: " << error_info << std::endl;
}

clz::VideoDecode::VideoDecode(QObject *parent) : QThread(parent)
{
    m_owidth = 800;
    m_oheight = 600;
    std::cout << "initialization ffmpeg [version]:" << avcodec_version() << std::endl;
}

void clz::VideoDecode::run()
{
    int count = 0;
    while (av_read_frame(m_avformat_context, m_av_packet) >= 0) {
        if(m_av_packet->stream_index == AVMEDIA_TYPE_VIDEO){
            // 匹配视频流
            count ++;
            int ret = avcodec_send_packet(m_avcodec_context, m_av_packet);
            ret = avcodec_receive_frame(m_avcodec_context, m_frame_in);
            if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
            if(ret < 0) break;

            sws_scale(m_sws_context, (const unsigned char* const*)m_frame_in->data, m_frame_in->linesize,
                      0,
                      m_avcodec_context->height,
                      m_frame_rgb->data, m_frame_rgb->linesize);
//            QImage image(output, m_avcodec_context->width, m_avcodec_context->height, QImage::Format_RGB32);
//            image.save(QString("D:/TEST/video%1.png").arg(count), "PNG", 100);
            emit sig_video_info_decoded((unsigned char*)m_outbuffer, m_owidth, m_oheight);
            msleep(17);
        }
        av_packet_unref(m_av_packet);
    }
}

void clz::VideoDecode::set_video_file(const QString &path)
{
    m_video_path = path;
}

void clz::VideoDecode::ffmpeg_init()
{
    avformat_network_init();

    m_avformat_context = avformat_alloc_context();
    {
        // 1, open video
        int avformat_open_result = avformat_open_input(&m_avformat_context, m_video_path.toStdString().c_str(), NULL, NULL);
        if(avformat_open_result != 0){
            //error
            check_ffmpeg_error(avformat_open_result, 1024);
        }
    }

    {
        // 2, find video stream info
        int avformat_find_stream_info_result = avformat_find_stream_info(m_avformat_context, NULL);
        if(avformat_find_stream_info_result < 0){
            check_ffmpeg_error(avformat_find_stream_info_result, 1024);
        }

        // 3, find video decoder
        int av_stream_index = -1;
        for(int i = 0, si = m_avformat_context->nb_streams; i < si; i++){
            if(m_avformat_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
                av_stream_index = i;
                break;
            }
        }
        if(av_stream_index == -1) {
            std::cout << "find video stream failed! " << std::endl;
            return;
        }

        auto avcodec_par = m_avformat_context->streams[av_stream_index]->codecpar;
        auto avcodec = avcodec_find_decoder(avcodec_par->codec_id);
        if(avcodec == NULL){
            std::cout << "find video decoder failed! " << std::endl;
            return;
        }

        m_avcodec_context = avcodec_alloc_context3(avcodec);

        // 4, synchronization avcodecparameters
        avcodec_parameters_to_context(m_avcodec_context, avcodec_par);
        // 5, open decoder
        int avcodec_open2_result = avcodec_open2(m_avcodec_context, avcodec, NULL);
        if(avcodec_open2_result != 0){
            check_ffmpeg_error(avcodec_open2_result, 1024);
        }


    }

    // 6, print dump infomation when exit
    av_dump_format(m_avformat_context, 0, m_video_path.toStdString().c_str(), 0);

    AVPixelFormat format = AV_PIX_FMT_BGR32;
    {
        // 7, do decode pre
        m_av_packet = av_packet_alloc();
        m_frame_in = av_frame_alloc();
        m_frame_rgb = av_frame_alloc();
        // 初始化缓冲区
        m_outbuffer = (uint8_t*)av_malloc(av_image_get_buffer_size(
                                          format,m_owidth, m_oheight,1));
        av_image_fill_arrays(m_frame_rgb->data, m_frame_rgb->linesize,
                             m_outbuffer, format, m_owidth, m_oheight, 1);

        // 转码初始化
        m_sws_context = sws_getContext(m_avcodec_context->width,
                                        m_avcodec_context->height,
                                        m_avcodec_context->pix_fmt,
                                        m_owidth, m_oheight,
                                        format, SWS_BICUBIC, NULL, NULL, NULL);

    }
}

void clz::VideoDecode::reset()
{

}

void clz::VideoDecode::close()
{
    av_packet_free(&m_av_packet);
    avcodec_close(m_avcodec_context);
    avformat_free_context(m_avformat_context);
}

void clz::VideoDecode::update_decode_size(int w, int h)
{
    m_owidth = w;
    m_oheight = h;
}
