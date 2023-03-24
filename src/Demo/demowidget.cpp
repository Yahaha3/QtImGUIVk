// 可能vulkan比较敏感?必须要放在最前面
//#include "common/Image.h"
#include "demowidget.h"
#include "implot.h"
#include <cmath>
#include <QFile>
#include <QCoreApplication>
#include <QVulkanFunctions>
#include "Map/GeoMap.h"
#include "backends/imgui_impl_vulkan.h"
#include "GeoFunction/GeoFunctionUtility.h"
#include "MapSupport/ClzMapSupport.h"
#include "MapSupport/CflxSupport.h"
#include "KernelSupport/KernelSupport.h"

#include "socket_function_include.h"
#include "DatabaseCflx/db_cflx_include.h"
#include "AosKernelCommon.h"

#include "ClzWidgets/ClzVLayout.h"
#include "ClzWidgets/ClzHLayout.h"
#include "Video/VideoWidget.h"
#include "ClzWidgets/ClzImageButton.h"
#include "ClzWidgets/ClzClickableFrame.h"

DemoWidget::DemoWidget(VulkanWindow *w, QWidget *parent) :ImGuiVulkanWidget(w, parent)
{
    set_clear_color(ImVec4(0.2f, 0.3f, 0.5f, 1));

    m_map = std::make_shared<clz::GeoMap>();
    m_cms = std::make_shared<clz::ClzMapSupport>();
    m_ccs = std::make_shared<clz::CflxSupport>();
    m_ks = std::make_shared<KernelSupport>();
    m_layout_left  = std::make_shared<clz::ClzVLayout>();
    m_layout_right = std::make_shared<clz::ClzVLayout>();

    m_layout_top_menu_entity_status = std::make_shared<clz::ClzHLayout>();
    m_layout_top_menu_system_status = std::make_shared<clz::ClzHLayout>();
    m_layout_bottom_function = std::make_shared<clz::ClzHLayout>();

    m_hero_card = std::make_shared<clz::ClzClickableFrame>();
    m_hero_card->set_widget_name("hero");
    m_cms->map_init("Vulkan SoloQ", m_map.get());
    m_ccs->map_init(m_map.get());

    kernel_init();
}

void DemoWidget::paint()
{
    paint_map();
}

void DemoWidget::init_window()
{
    ImPlot::CreateContext();
    ImPlot::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(0,0);
    style.FramePadding = ImVec2(0,0);
    style.FrameBorderSize = 0;
    style.WindowBorderSize = 1.0f;
}

void DemoWidget::release_window()
{
    ImPlot::DestroyContext();
}

void DemoWidget::vulkan_window_ready()
{
    // 布局初始化要在ImGui上下文准备完成之后
    layout_init();
    m_map->sub_items_init();
    // ui初始化
    m_ks->ui_init(m_layout_bottom_function.get(),
                  m_layout_right.get(),
                  m_layout_top_menu_entity_status.get());
}

void DemoWidget::paint_map()
{
    if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_A))){
        debug = !debug;
    }
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    const ImU32 flags = ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin("MapFrame", 0, flags);
    if(debug){
        ImGui::Text("FPS: %.2f Renders: %d", ImGui::GetIO().Framerate, renders);
    }

    ImPlotFlags plot_flags = ImPlotFlags_NoTitle|
            ImPlotFlags_NoLegend|
            ImPlotFlags_NoMouseText|
            ImPlotFlags_NoFrame|
            ImPlotFlags_FullScreen|
            ImPlotFlags_Equal;
    ImPlotAxisFlags ax_flags = ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels |
                               ImPlotAxisFlags_NoGridLines| ImPlotAxisFlags_Foreground|
                               ImPlotAxisFlags_NoTickMarks;
    if(ImPlot::BeginPlot("##Map",viewport->WorkSize, plot_flags)){
        ImPlot::SetupAxes(NULL,NULL,ax_flags,ax_flags|ImPlotAxisFlags_Invert);
        ImPlot::SetupAxesLimits(0,1,0,1);
        m_map->map_timeout();
        ImPlot::PushPlotClipRect();
//        m_map->test();
        layout_update();
        hero_card_update();
        ImPlot::PopPlotClipRect();
        ImPlot::EndPlot();
    }
    ImGui::End();

//    ImGui::ShowDemoWindow();
}


void DemoWidget::kernel_init()
{
    DataTransmissior::handle()->set_share_mem_use(false);
    DataTransmissior::handle()->init(eqnx::other::cflx_channel);
    QString data = TransmissiorUnInited;
    while(data == TransmissiorDisConnected || data == TransmissiorUnInited){
        data = DataTransmissior::handle()->read_data(eqnx::other::server_name);
        std::cout << "reading redis..." << std::endl;
    }

    ConfluxHelper::instance()->init(aos::AosKernelCommon::string_to_json_object(data));

//    connect(DataTransmissior::handle(), &DataTransmissior::sig_mem_changed, this, &AppWnd::slot_update_share_mem);

    m_ks->init();

    // 初始化航线
    {
        m_ccs->init_download_mission_phases(ConfluxHelper::instance()->actors().first());
    }
}


void DemoWidget::layout_init()
{
    {
        auto layout = clz::LayoutAnchor::Left | clz::LayoutAnchor::VCenter;
        m_layout_left->set_layout_anchor(static_cast<clz::LayoutAnchor>(layout));

        layout = clz::LayoutAnchor::Right | clz::LayoutAnchor::Top;
        m_layout_right->set_layout_anchor(static_cast<clz::LayoutAnchor>(layout));

        layout = clz::LayoutAnchor::Top | clz::LayoutAnchor::Left;
        m_layout_top_menu_entity_status->set_layout_anchor(static_cast<clz::LayoutAnchor>(layout));

        layout = clz::LayoutAnchor::Bottom | clz::LayoutAnchor::HCenter;
        m_layout_bottom_function->set_layout_anchor(static_cast<clz::LayoutAnchor>(layout));
    }

    const QString side_path = "D:/github/QtImGUIVk/resources/icon/side_bar/black/%1_44x44.png";
    {
        // 左侧功能面板初始化, 磁吸/hud/感知域/定位/地图缩/地图放/设置轨迹存续时间/清除轨迹/测距
        m_layout_left->set_contents_margins(1, 5, 1, 5);
        m_layout_left->set_sensitive_width_fold(false);
        auto centered       = new clz::ClzImageButton(side_path.arg("centered"), "centered");
        auto hud            = new clz::ClzImageButton(side_path.arg("overlay-hud"), "overlay-hud");
        auto projection     = new clz::ClzImageButton(side_path.arg("projection"), "projection");
        auto position       = new clz::ClzImageButton(side_path.arg("position"), "position");
        auto zoom_in        = new clz::ClzImageButton(side_path.arg("zoom-in"), "zoom_in");
        auto zoom_out       = new clz::ClzImageButton(side_path.arg("zoom-out"), "zoom_out");
        auto track          = new clz::ClzImageButton(side_path.arg("track"), "track");
        auto delete_track   = new clz::ClzImageButton(side_path.arg("delete-track"), "delete_track");
        auto measure        = new clz::ClzImageButton(side_path.arg("measure"), "measure");
        m_layout_left->add_widget(centered);
        m_layout_left->add_widget(hud);
        m_layout_left->add_widget(projection);
        m_layout_left->add_widget(position);
        m_layout_left->add_widget(zoom_in);
        m_layout_left->add_widget(zoom_out);
        m_layout_left->add_widget(track);
        m_layout_left->add_widget(delete_track);
        m_layout_left->add_widget(measure);
    }

    {
//        showMaximized();
    }
}

void DemoWidget::layout_update()
{
    m_layout_left->relayout();
    m_layout_right->relayout();
    m_layout_top_menu_entity_status->relayout();
//    m_layout_top_menu_system_status->relayout();
    m_layout_bottom_function->relayout();
}
#include "KernelSupport/KernelInstanceData.h"
void DemoWidget::hero_card_update()
{
    bool ok = false;
    auto instance = m_ks->instance_data()->current_instance_data(ok);
    if(!ok) return;
    m_hero_card->update_left_info("Auto");
    m_hero_card->update_right_info(instance.entity_name);
    m_hero_card->set_background_image(instance.entity_image);
    m_hero_card->paint();
}

void DemoWidget::resizeEvent(QResizeEvent *e)
{
    auto msize = this->size();

    {
        m_layout_left->resize_event(msize);
        m_layout_right->resize_event(msize);
        m_layout_top_menu_entity_status->resize_event(msize);
//        m_layout_top_menu_system_status->resize_event(msize);
        m_layout_bottom_function->resize_event(msize);
    }

    if(m_hero_card){
        m_hero_card->move(QPoint(1, msize.height() - m_hero_card->size().height()));
    }
    ImGuiVulkanWidget::resizeEvent(e);
}

