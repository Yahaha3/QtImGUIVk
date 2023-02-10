#include "GraphicsLayer.h"

clz::GraphicsLayer::GraphicsLayer(const QString &name, const QString &description)
    : m_name(name)
    , m_desccription(description)
{

}

void clz::GraphicsLayer::set_name(const QString &name)
{
    m_name = name;
}

QString clz::GraphicsLayer::get_name() const
{
    return m_name;
}

void clz::GraphicsLayer::set_description(const QString &description)
{
    m_desccription = description;
}

QString clz::GraphicsLayer::get_description() const
{
    return m_desccription;
}
