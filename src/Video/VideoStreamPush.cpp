#include "VideoStreamPush.h"
#include <iostream>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
    #include <libavutil/avutil.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/time.h>
}

extern void check_ffmpeg_error(int result, size_t size);

clz::VideoStreamPush::VideoStreamPush(QObject *parent) : QThread(parent)
{

}

void clz::VideoStreamPush::run()
{
    long long start_time = av_gettime();
    while (av_read_frame(m_avformat_context_input, m_av_packet) >= 0) {
        auto index = m_av_packet->stream_index;
        auto itime = m_avformat_context_input->streams[index]->time_base;
        auto otime = m_avformat_context_output->streams[index]->time_base;
        m_av_packet->pts = av_rescale_q_rnd(m_av_packet->pts, itime, otime, AVRounding(AV_ROUND_INF | AV_ROUND_INF));
        m_av_packet->dts = av_rescale_q_rnd(m_av_packet->dts, itime, otime, AVRounding(AV_ROUND_INF | AV_ROUND_INF));
        m_av_packet->duration = av_rescale_q_rnd(m_av_packet->duration, itime, otime, AVRounding(AV_ROUND_INF | AV_ROUND_INF));
        m_av_packet->pos = -1;
        auto avcodec_par = m_avformat_context_input->streams[index]->codecpar;
        if(avcodec_par->codec_type == AVMEDIA_TYPE_VIDEO){
            auto tb = m_avformat_context_input->streams[index]->time_base;
            long long now = av_gettime() - start_time;
            long long dts = 0;
            dts = m_av_packet->dts * (1000 * 1000 * av_q2d(tb));
            if(dts > now){
                av_usleep(dts - now);
            }
        }

        auto ret = av_interleaved_write_frame(m_avformat_context_output, m_av_packet);
        if(ret < 0) break;
        av_packet_unref(m_av_packet);
    }
}

void clz::VideoStreamPush::close()
{
    av_packet_free(&m_av_packet);
    avcodec_close(m_avcodec_context);
    avformat_free_context(m_avformat_context_input);
    avformat_free_context(m_avformat_context_output);
}

void clz::VideoStreamPush::ffmpeg_init()
{
    avformat_network_init();

    {
        // input
        m_avformat_context_input = avformat_alloc_context();
        auto ret = avformat_open_input(&m_avformat_context_input, m_src.toStdString().c_str(), NULL, NULL);
        if(ret != 0){
            check_ffmpeg_error(ret, 1024);
        }

        ret = avformat_find_stream_info(m_avformat_context_input, NULL);
        if(ret != 0){
            check_ffmpeg_error(ret, 1024);
        }

        av_dump_format(m_avformat_context_input, 0, m_src.toStdString().c_str(), 0);

    }

    {
        // output
        auto ret = avformat_alloc_output_context2(&m_avformat_context_output, NULL,
                                                  m_protocol.toStdString().c_str(),
                                                  m_des.toStdString().c_str());
        if(ret != 0){
            check_ffmpeg_error(ret, 1024);
        }

        for(int i = 0, si = m_avformat_context_input->nb_streams; i < si; i++){
            auto avcodec_par = m_avformat_context_input->streams[i]->codecpar;
            auto avcodec = avcodec_find_decoder(avcodec_par->codec_id);
            auto ostream = avformat_new_stream(m_avformat_context_output, avcodec);
            if(!ostream) return;

            ret = avcodec_parameters_copy(ostream->codecpar,  avcodec_par);
            if(ret < 0) return;

            ostream->codecpar->codec_tag = 0;
        }

        av_dump_format(m_avformat_context_output, 0, m_des.toStdString().c_str(), 0);
    }

    {
        // open
        auto ret = avio_open(&m_avformat_context_output->pb, m_des.toStdString().c_str(), AVIO_FLAG_WRITE);
        if(ret < 0) return;

        ret = avformat_write_header(m_avformat_context_output, NULL);
        if(ret < 0) return;
    }
}

void clz::VideoStreamPush::set_src_path(const QString &path)
{
    m_src = path;
}

void clz::VideoStreamPush::set_des_path(const QString &path)
{
    m_des = path;
}

void clz::VideoStreamPush::set_protocol(const QString &protocol)
{
    m_protocol = protocol;
}
