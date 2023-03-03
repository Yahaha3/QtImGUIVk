#ifndef DEMOWIDGET_H
#define DEMOWIDGET_H

#include "common/vulkanwindow.h"
#include "Video/VideoDecode.h"

namespace clz {
class GeoMap;
class ImageDrawElement;
class ClzMapSupport;
}
//class ImageVk;

class DemoWidget : public ImGuiVulkanWidget
{
public:
    DemoWidget(VulkanWindow* w, QWidget* parent = NULL);

protected:
    void paint() override;
    void init_window() override;
    void release_window() override;
    void vulkan_window_ready() override;
private:
    void paint1();
    void paint2();
    void paint_map();
    void paint_video();
    void decode_video();

    void kernel_init();

private slots:
    void slot_video_decode_info(uchar* data, int w, int h);
private:
    ImVec4 _bcolor;
    QString log;
    int renders = 0;
    bool debug = false;

//    std::shared_ptr<ImageVk> image_vk;

    std::shared_ptr<clz::GeoMap> m_map;

    std::shared_ptr<clz::VideoDecode> m_video_decode;

    // test
    clz::ImageDrawElement* m_video = nullptr;

    std::shared_ptr<clz::ClzMapSupport> m_cms;
};

#endif // DEMOWIDGET_H
