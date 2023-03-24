#include "ClzCardWidget.h"
#include <QJsonArray>
#include <QJsonObject>

clz::ClzCardWidget::ClzCardWidget(clz::ClzWidget *parent):ClzWidget(parent)
{

}

bool clz::ClzCardWidget::init_card(const QStringList &context, const QStringList &uri, const QJsonArray &options)
{
    auto size = context.size();
    if(uri.size() == size && options.size() == size){
        // first clear
        m_context.clear();
        m_value_map.clear();
        m_context_option_map.clear();
        // second fill
        m_context = context;
        for(int i = 0, si = uri.size(); i < si; i++){
            m_value_map.insert(uri[i], "N/A");
            m_context_uri_map.insert(context[i], uri[i]);
            m_context_option_map.insert(context[i], options[i].toObject());
        }
        resize(QSize(300, (m_context.size() + 1) * 17));
        return true;
    }
    return false;
}

bool clz::ClzCardWidget::update_card(const QString &uri, QJsonValue value)
{
    if(!m_value_map.keys().contains(uri)) return false;

    auto index = m_value_map.keys().indexOf(uri);
    auto option = m_context_option_map[m_context[index]];
    if(!option.isEmpty() && value.isDouble()){
        // 需要做枚举
    } else {
        // 不需要做枚举
        if(value.isDouble()){
            // 可能有位数保留
            m_value_map[uri] = QString::number(value.toDouble(), 'f', 2);
        } else if(value.isString()){
            m_value_map[uri] = value.toString();
        }
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
    ImGui::Begin(name().toStdString().c_str(), 0);

    m_widget_fold = ImGui::IsWindowCollapsed();
    for(int i = 0, si = m_context.size(); i < si; i++){
        auto context = m_context[i];
        auto value = m_value_map[m_context_uri_map[context]];
        ImGui::Text(context.toStdString().c_str(), "");
        ImGui::SameLine();
        ImGui::Text(":");
        ImGui::SameLine();
        ImGui::Text(value.toStdString().c_str(), "");
    }

    ImGui::End();
}

void clz::ClzCardWidget::slot_card_data_update(const QString &uri, QJsonValue value)
{
    update_card(uri, value);
}
