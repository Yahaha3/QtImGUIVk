#include "ClzWidget.h"

clz::ClzWidget::ClzWidget(clz::ClzWidget *parent): ClzObject(parent)
{

}

QPoint clz::ClzWidget::pos()
{
    return m_position;
}

QSize clz::ClzWidget::size()
{
    return m_size;
}

QString clz::ClzWidget::title()
{
    return m_title;
}

void clz::ClzWidget::set_widget_title(const QString &title)
{
    m_title = title;
}

void clz::ClzWidget::move(QPoint position)
{
    m_position = position;
}

void clz::ClzWidget::resize(QSize size)
{
    m_size = size;
}
