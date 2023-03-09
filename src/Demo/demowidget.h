#ifndef DEMOWIDGET_H
#define DEMOWIDGET_H

#include "common/vulkanwindow.h"
#include "Video/VideoDecode.h"

namespace clz {
class GeoMap;
class ImageDrawElement;
class ClzMapSupport;
class CflxSupport;
}

/* 该项目分为以下几个模块,在后续应用到项目后需要拆分为子模块独立维护
 * 0, common是通用模块
 * 1, 地图模块:
 *      地图模块包括 ELement/GeoFunction/Map/MapThread/Operator
 *      上述内容为地图的基本构成
 * 2, 地图支持模块:
 *      地图支持模块主要用于业务实现
 *      内容包括 MapSupport
 * 3, App组件模块
 *      App组件主要用于呈现各显示元素,包括Hud,视频,其他功能按钮以及状态栏等,宏观上来说,地图模块也属于App组件
 *      内容包括 Video/etc..
 *      Todo: 首先需要实现Layout布局
 *
*/

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

    void kernel_init();

private:
    ImVec4 _bcolor;
    QString log;
    int renders = 0;
    bool debug = false;

//    std::shared_ptr<ImageVk> image_vk;

    std::shared_ptr<clz::GeoMap> m_map;

    std::shared_ptr<clz::ClzMapSupport> m_cms;
    std::shared_ptr<clz::CflxSupport> m_ccs;
};

#endif // DEMOWIDGET_H
