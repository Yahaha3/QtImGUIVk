#ifndef DEMOWIDGET_H
#define DEMOWIDGET_H

#include "common/vulkanwindow.h"
#include "Video/VideoDecode.h"

namespace clz {
class GeoMap;
class ImageDrawElement;
class ClzMapSupport;
class CflxSupport;
class ClzVLayout;
class ClzHLayout;
class ClzClickableFrame;
}

class KernelSupport;

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
 * 4, 数据库模块
 *      为地图提供瓦片缓存业务
 *      内容包括 Database
 * 5, 内核支持模块
 *      内核支持模块用于解析并存储相关数据,包括卡片信息,功能按钮能
 *      内容包括 KernelSupport
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

    void layout_init();
    void layout_update();
    void hero_card_update();

    void resizeEvent(QResizeEvent* e) override;

private:
    ImVec4 _bcolor;
    QString log;
    int renders = 0;
    bool debug = false;

//    std::shared_ptr<ImageVk> image_vk;

    std::shared_ptr<clz::GeoMap> m_map;

    std::shared_ptr<clz::ClzMapSupport> m_cms;
    std::shared_ptr<clz::CflxSupport> m_ccs;
    std::shared_ptr<KernelSupport> m_ks;

    // 右侧卡片面板
    std::shared_ptr<clz::ClzVLayout> m_layout_right;
    // 底部功能按钮
    std::shared_ptr<clz::ClzHLayout> m_layout_bottom_function;
    // 左下角卡片
    std::shared_ptr<clz::ClzClickableFrame> m_hero_card;
    // 左侧功能按钮
    std::shared_ptr<clz::ClzVLayout> m_layout_left;
    // 顶部状态栏
    std::shared_ptr<clz::ClzHLayout> m_layout_top_menu_entity_status;
    std::shared_ptr<clz::ClzHLayout> m_layout_top_menu_system_status;
};

#endif // DEMOWIDGET_H
