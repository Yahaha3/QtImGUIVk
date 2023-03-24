#include "ClzImageButton.h"

clz::ClzImageButton::ClzImageButton(clz::ClzWidget *parent) : ClzWidget(parent)
{
    m_window_flags = ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBackground;

    m_btn_image = std::make_shared<clz::ImageDrawElement>();
}

clz::ClzImageButton::ClzImageButton(const QString &image, const QString& name, clz::ClzWidget *parent) : ClzWidget(parent)
{
    resize(QSize(44,44));
    m_window_flags = ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBackground;
    set_widget_name(name);
    m_btn_image = std::make_shared<clz::ImageDrawElement>();
    set_image_path(image);
}

void clz::ClzImageButton::set_image_path(const QString &image)
{
    if(m_image_path != image){
        bool ok = false;
        auto array = clz::GeoFunctionUtility::get_image_bytearray(image, ok);
        if(ok){
            m_btn_image->load_image(array);
        }
        m_image_path = image;
    }
}

void clz::ClzImageButton::init()
{
    auto path = "E:/Phugia/aOS-kernel/bin/win64/debug/installer/UX/csgrs/auto_64x64.png";
    set_image_path(path);
}

void clz::ClzImageButton::paint()
{
    ImGui::SetNextWindowPos(impos());
    ImGui::SetNextWindowSize(imsize());
    ImGui::Begin(name().toStdString().c_str(), 0, m_window_flags);
    if(m_btn_image){
        auto ID = m_btn_image->get_image_texture_id();

        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(100, 150, 250, 20));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(100, 150, 250, 150));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(100, 150, 250, 250));

        if(ImGui::ImageButton(ID, imsize(), {0,0}, {1,1}, -1)){
            std::cout << "image btn clicked: " << m_name.toStdString() << std::endl;
            emit sig_image_btn_clicked(get_object_id());
        }

        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::Text(name().toStdString().c_str(), "");
    }
    ImGui::End();
}
