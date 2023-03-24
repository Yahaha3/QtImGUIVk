#include "VideoWidget.h"

clz::VideoWidget::VideoWidget(clz::ClzWidget *parent) : ClzWidget(parent)
{
    m_video = std::make_shared<clz::ImageDrawElement>();
    m_video_decode = std::make_shared<clz::VideoDecode>();
    connect(m_video_decode.get(), &clz::VideoDecode::sig_video_info_decoded, this, &VideoWidget::slot_video_decode_info);
}

void clz::VideoWidget::init()
{
    m_video_decode->set_video_file("D:/TEST/20230201_164749.mp4");
    m_video_decode->update_decode_size(size().width(), size().height() - 38);
    m_video_decode->ffmpeg_init();
    m_video_decode->start();
}

void clz::VideoWidget::paint()
{
    ImGui::SetNextWindowPos(impos());
    ImGui::SetNextWindowSize(imsize());
    ImGui::Begin(name().toStdString().c_str(), 0);
    m_widget_fold = ImGui::IsWindowCollapsed();
    if(!m_video) return;
    auto ID = m_video->get_image_texture_id();
    if(ID){
        ImGui::Image(ID, {(float)m_video->width(), (float)m_video->height()});
    }
    ImGui::End();
}

void clz::VideoWidget::slot_video_decode_info(uchar *data, int w, int h)
{
    if(m_video){
        m_video->update_image(data, w, h);
    }
}
