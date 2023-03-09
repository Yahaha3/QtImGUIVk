#include "ClzLayout.h"
#include "ClzCore/ClzObjectManager.h"

clz::ClzLayout::ClzLayout(clz::ClzObject *parent) : ClzObject(parent)
{
    m_margin_left       = 0;
    m_margin_right      = 0;
    m_margin_top        = 0;
    m_margin_bottom     = 0;
}

void clz::ClzLayout::set_contents_margins(int left, int top, int right, int bottom)
{
    m_margin_left       = left;
    m_margin_right      = right;
    m_margin_top        = top;
    m_margin_bottom     = bottom;
}

void clz::ClzLayout::add_widget(clz::ClzWidget *widget)
{
    if(contain_widget(widget)) return;
    m_widgets.append(widget->get_object_id());
}

void clz::ClzLayout::remove_widget(clz::ClzWidget *widget)
{
    if(!contain_widget(widget)) return;
    m_widgets.removeAll(widget->get_object_id());
}

bool clz::ClzLayout::contain_widget(clz::ClzWidget *widget)
{
    return m_widgets.contains(widget->get_object_id());
}

QPoint clz::ClzLayout::pos()
{
    return m_position;
}

QSize clz::ClzLayout::size()
{
    return m_size;
}

void clz::ClzLayout::move(QPoint position)
{
    m_position = position;
}

void clz::ClzLayout::resize(QSize size)
{
    m_size = size;
}
