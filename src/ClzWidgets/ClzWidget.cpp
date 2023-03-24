#include "ClzWidget.h"

clz::ClzWidget::ClzWidget(clz::ClzWidget *parent): ClzObject(parent)
{
}

ImVec2 clz::ClzWidget::impos()
{
    return ImVec2((float)m_position.x(), (float)m_position.y());
}

QPoint clz::ClzWidget::pos()
{
    return m_position;
}

QSize clz::ClzWidget::size()
{
    return m_size;
}

ImVec2 clz::ClzWidget::imsize()
{
    return ImVec2((float)m_size.width(), (float)m_size.height());
}

QString clz::ClzWidget::name()
{
    return m_name;
}

bool clz::ClzWidget::fold()
{
    return m_widget_fold;
}

void clz::ClzWidget::set_widget_name(const QString &name)
{
    m_name = name;
}

void clz::ClzWidget::move(QPoint position)
{
    m_position = position;
}

void clz::ClzWidget::resize(QSize size)
{
    m_size = size;
}
