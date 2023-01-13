#ifndef DEMOWIDGET_H
#define DEMOWIDGET_H

//#include "GeoMap.h"
#include "common/vulkanwindow.h"

class ImageVk;

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

private:
    bool _open = false;
    ImVec4 _bcolor;
    QString log;
    int renders = 0;
    bool debug = false;

    std::shared_ptr<ImageVk> image_vk;

//    std::shared_ptr<clz::GeoMap> m_map;
};

#endif // DEMOWIDGET_H
