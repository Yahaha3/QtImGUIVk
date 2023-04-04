#include "ClzCardWidget.h"
#include "GeoFunction/GeoFunctionUtility.h"
#include <QJsonArray>
#include <QJsonObject>

clz::ClzCardWidget::ClzCardWidget(clz::ClzWidget *parent):ClzWidget(parent)
{
    m_window_flags = ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoScrollbar |
                    ImGuiWindowFlags_NoMove;
}

bool clz::ClzCardWidget::init_card(const QStringList &context, const QStringList &uri, const QStringList &formats, const QList<QJsonArray> &options)
{
    auto size = context.size();
    if(uri.size() == size && options.size() == size && formats.size() == size){
        // first clear
        m_context.clear();
        m_value_map.clear();
        m_context_option_map.clear();
        // second fill
        m_context = context;
        m_uris = uri;
        for(int i = 0, si = uri.size(); i < si; i++){
            m_value_map.insert(uri[i], "N/A");
            m_context_uri_map.insert(context[i], uri[i]);
            m_context_option_map.insert(context[i], options[i]);
            m_context_format_map.insert(context[i], formats[i]);
        }
        resize(QSize(300, (m_context.size() + 1) * 20));
        return true;
    }
    return false;
}

bool clz::ClzCardWidget::update_card(const QString &uri, QJsonValue value)
{
    if(!m_value_map.keys().contains(uri)) return false;

    auto index = m_uris.indexOf(uri);
    auto option = m_context_option_map[m_context[index]];
    auto format = m_context_format_map[m_context[index]];
    bool ok = false;
    QString result = clz::GeoFunctionUtility::parse_enum(value, option, ok);
    if(!ok){
        result = clz::GeoFunctionUtility::parse_format(value, format, ok);
    }

    if(ok){
        m_value_map[uri] = result;
    }
    return true;
}

void clz::ClzCardWidget::init()
{

}

void clz::ClzCardWidget::paint()
{
    ImGui::SetNextWindowPos(impos());
    ImGui::SetNextWindowSize(imsize());
    ImVec2 alignment = ImVec2(0.5, 0.5);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, alignment);
    ImGui::Begin(name().toStdString().c_str(), 0, m_window_flags);
    ImGui::SetWindowFontScale(1.27);
    m_widget_fold = ImGui::IsWindowCollapsed();
    auto window_width = ImGui::GetWindowSize().x;
    auto colon_width = ImGui::CalcTextSize(m_colon.toStdString().c_str()).x;
    auto center = (window_width - colon_width) * 0.5f;
    for(int i = 0, si = m_context.size(); i < si; i++){
        auto context = m_context[i];
        auto value = m_value_map[m_context_uri_map[context]];
        auto context_width = ImGui::CalcTextSize(context.toStdString().c_str()).x;
        ImGui::SetCursorPosX(center - context_width);
        ImGui::Text(context.toStdString().c_str(), "");
        ImGui::SameLine();
        ImGui::Text(":");
        ImGui::SameLine();
        ImGui::Text(value.toStdString().c_str(), "");
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

void clz::ClzCardWidget::slot_card_data_update(const QString &uri, QJsonValue value)
{
    update_card(uri, value);
}
