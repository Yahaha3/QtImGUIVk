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

#include "socket_function_include.h"
#include "DatabaseCflx/db_cflx_include.h"
#include "AosKernelCommon.h"

//using namespace ImGui;

DemoWidget::DemoWidget(VulkanWindow *w, QWidget *parent) :ImGuiVulkanWidget(w, parent)
{
    set_clear_color(ImVec4(0.2f, 0.3f, 0.5f, 1));

//    image_vk = std::make_shared<ImageVk>();
    m_map = std::make_shared<clz::GeoMap>();
    m_cms = std::make_shared<clz::ClzMapSupport>();
    m_ccs = std::make_shared<clz::CflxSupport>();
    m_cms->map_init("Vulkan SoloQ", m_map.get());
    m_ccs->map_init(m_map.get());

//    kernel_init();
}

void DemoWidget::paint()
{
//    paint1();
//    paint2();
    paint_map();
}

void DemoWidget::init_window()
{
    ImPlot::CreateContext();
    ImPlot::StyleColorsDark();
}

void DemoWidget::release_window()
{
    ImPlot::DestroyContext();
}

void DemoWidget::vulkan_window_ready()
{
//    auto path = QCoreApplication::applicationDirPath() + "/image.png";
//    QFile file(path);
//    if (file.open(QFile::ReadOnly))
//    {
//        auto array = file.readAll();
//        image_vk->LoadFromBytearry(array.data(), array.size());
//    }
}

void DemoWidget::paint1()
{
    bool open = false;
    ImGui::Begin(u8"你好!");

//    ImGui::StyleColorsLight();

    ImGui::Text("This is some useful text.");
//    ImGui::SameLine();
//    ImGui::Indent();
    ImGui::Checkbox("Color Widget", &open);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if(open)
    {
        ImGui::Begin("Color Select", &open);
        ImGui::Text("Color Select!!!");
        ImGui::ColorPicker4("clear color", (float*)&_bcolor);
        set_clear_color(_bcolor);
        ImGui::End();
    }

    ImGui::Separator();
    if(ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){
        auto style = ImGui::GetStyle();
        if (ImGui::BeginTabItem("Size1"))
        {
            ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Size2"))
        {
            ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void DemoWidget::paint2()
{
    bool open = false;
    ImGui::Begin("First Tool", &open, ImGuiWindowFlags_MenuBar);
    if(ImGui::BeginMenuBar()){
        if(ImGui::BeginMenu("File")){
            if(ImGui::MenuItem("Open...", "Ctrl+O")){
                log = "do open";
            }
            if(ImGui::MenuItem("Save...", "Ctrl+S")){
                log = "do save";
            }
            if(ImGui::MenuItem("Close...", "Ctrl+W")){
                log = "do close";
                open = false;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::Text("%s", log.toLatin1().data());

    ImVec4 color;
    ImGui::ColorEdit4("Color", (float*)&color);

    float samples[100];
    for(int i = 0; i < 100; i++){
        double value = i * 0.2f + ImGui::GetTime() *1.5f;
        samples[i] = sin(value);
    }
    ImGui::PlotLines("Samples", samples, 100/*, 50, NULL, 50, 50, ImVec2{300, 50}*/);
    ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
    ImGui::BeginChild("Scrolling");
    for(int i = 0; i < 50; i++){
        ImGui::Text("%04d: Some text", i);
    }
    ImGui::EndChild();

    ImGui::End();
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
        m_map->test();
        ImPlot::PopPlotClipRect();
        ImPlot::EndPlot();
    }
    ImGui::End();

//    ImPlot::ShowDemoWindow();
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

    auto inst = eqnx_dh::InstanceContainer::handle();
    inst->check_environmental();

    // 初始化航线
    {
//        m_ccs->init_download_mission_phases(ConfluxHelper::instance()->actors().first());
    }
}

