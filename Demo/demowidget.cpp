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

//using namespace ImGui;

DemoWidget::DemoWidget(VulkanWindow *w, QWidget *parent) :ImGuiVulkanWidget(w, parent)
{
    set_clear_color(ImVec4(0.2f, 0.3f, 0.5f, 1));

//    image_vk = std::make_shared<ImageVk>();
    m_map = std::make_shared<clz::GeoMap>();
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
    ImGui::Begin(u8"你好!");

//    ImGui::StyleColorsLight();

    ImGui::Text("This is some useful text.");
//    ImGui::SameLine();
//    ImGui::Indent();
    ImGui::Checkbox("Color Widget", &_open);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if(_open)
    {
        ImGui::Begin("Color Select", &_open);
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
    ImGui::Begin("First Tool", &_open, ImGuiWindowFlags_MenuBar);
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
                _open = false;
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

    ImGui::SetNextWindowPos({0,0});
    ImGui::SetNextWindowSize(ImVec2(this->size().width(), this->size().height()));
    ImGui::Begin("Map", 0, ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoTitleBar);
    if(debug){
        ImGui::Text("FPS: %.2f Renders: %d", ImGui::GetIO().Framerate, renders);
    }

    ImPlotAxisFlags ax_flags = ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels |
                               ImPlotAxisFlags_NoGridLines| ImPlotAxisFlags_Foreground;
    if(ImPlot::BeginPlot("##Map",ImVec2(-1,-1), ImPlotFlags_Equal | ImPlotFlags_NoMouseText)){
        ImPlot::SetupAxes(NULL,NULL,ax_flags,ax_flags|ImPlotAxisFlags_Invert);
        ImPlot::SetupAxesLimits(0,1,0,1);
        m_map->update_tiles();
        ImPlot::PushPlotClipRect();
        m_map->test();
        m_map->update_items();
        ImPlot::PopPlotClipRect();
        ImPlot::EndPlot();
    }
//    ImPlot::ShowDemoWindow();
//    ImGui::ShowDemoWindow();
    ImGui::End();
}

