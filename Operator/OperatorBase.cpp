#include "OperatorBase.h"
#include "Element/ElementContainer.h"

clz::OperatorBase::OperatorBase(clz::GeoMap *map, QString signature, QObject *parent): QObject(parent)
  ,m_map(map), m_visible(true), m_signature(signature)
{

}

clz::OperatorBase::~OperatorBase()
{

}

clz::GeoMap *clz::OperatorBase::map() const
{
    return m_map;
}

bool clz::OperatorBase::is_valid() const
{
    return m_is_valid;
}

clz::ClzOpResult clz::OperatorBase::generate_operator(const clz::GeoPos &position, bool noted)
{
    Q_UNUSED(noted);
    m_center = position;

    return Success;
}

clz::ClzOpResult clz::OperatorBase::generate_operator(const QVector<clz::GeoPos> &vertices, bool noted)
{
    Q_UNUSED(noted);
    update_vertices(vertices);
    return Success;
}

void clz::OperatorBase::destroy_operator(bool noted)
{
    Q_UNUSED(noted);
}

clz::ClzOpResult clz::OperatorBase::translate_to(clz::GeoPos position)
{
    Q_UNUSED(position);
    return Success;
}

void clz::OperatorBase::translate(clz::GeoPos offset)
{
    Q_UNUSED(offset);
}

void clz::OperatorBase::rotate(double angle)
{
    Q_UNUSED(angle);
}

void clz::OperatorBase::update_vertices(QVector<clz::GeoPos> vertices, bool noted)
{
    Q_UNUSED(noted);
    m_vertices.clear();
    m_vertices << vertices;
}

clz::ClzOpResult clz::OperatorBase::update_vertex(clz::GeoPos position, int index)
{
    if(index < m_vertices.size()){
        m_vertices[index] = position;
        return Success;
    }
    return OutOfRange;
}

clz::ClzOpResult clz::OperatorBase::add_vertex(clz::GeoPos position, int index)
{
    m_vertices.insert(index, position);
    return Success;
}

clz::ClzOpResult clz::OperatorBase::remove_vertex(int index)
{
    if(index < m_vertices.size()){
        m_vertices.removeAt(index);
        return Success;
    }
    return OutOfRange;
}

void clz::OperatorBase::append_vertex(clz::GeoPos position)
{
    m_vertices.append(position);
}

bool clz::OperatorBase::visible() const
{
    return m_visible;
}

void clz::OperatorBase::set_visible(bool visible)
{
    m_visible = visible;
}

QString clz::OperatorBase::signature() const
{
    return m_signature;
}

void clz::OperatorBase::set_icon_template(const QString icon)
{
    m_icon_template = icon;
}

QString clz::OperatorBase::icon_template() const
{
    return m_icon_template;
}

clz::GraphicsLayer *clz::OperatorBase::layer() const
{
    return m_layer;
}

clz::GraphicsDrawElement *clz::OperatorBase::element_body()
{
    auto body = ElementContainer::instance().get_element(m_element_body);
    return dynamic_cast<GraphicsDrawElement*>(body);
}

clz::GraphicsDrawElement *clz::OperatorBase::element_center()
{
    auto center = ElementContainer::instance().get_element(m_element_center);
    return dynamic_cast<GraphicsDrawElement*>(center);
}

clz::GraphicsDrawElement *clz::OperatorBase::element_vertex(int index)
{
    if(index >= m_element_vertices.size()) return nullptr;
    auto vertex = ElementContainer::instance().get_element(m_element_vertices[index]);
    return dynamic_cast<GraphicsDrawElement*>(vertex);
}

void clz::OperatorBase::refresh()
{

}
