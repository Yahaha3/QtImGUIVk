#include "GraphicsElement.h"
#include "ElementContainer.h"
#include "Map/GeoMap.h"
#include "Layer/GraphicsLayer.h"

clz::GraphicsElement::GraphicsElement(GraphicsElement *parent) : QObject(parent)
{
    if(parent){
        m_parent_id = parent->get_element_id();
    }
    set_visible(false);
    set_selected(false);
    set_selectable(false);

    m_type = clz::empty;
    set_element_id();
    clz::ElementContainer::instance().add_element(get_element_id(), this);
}

clz::GraphicsElement::~GraphicsElement()
{
    delete_elements();
    auto parent = clz::ElementContainer::instance().get_element(m_parent_id);
    if(parent != nullptr){
        parent->m_childrens.remove(get_element_id());
    }
    clz::ElementContainer::instance().remove_element(get_element_id());
}

void clz::GraphicsElement::set_parent(clz::GraphicsElement *parent)
{
    if(parent && parent->get_element_id() == m_parent_id) return;
    set_selected(false);
    auto parent_bef = clz::ElementContainer::instance().get_element(m_parent_id);
    if(parent_bef != nullptr){
        parent_bef->m_childrens.remove(get_element_id());
    }

    m_parent_id = parent->get_element_id();
    parent->m_childrens.insert(get_element_id());
    // need update ?

//    auto map = get_map();
//    if(!map) return;
//    map->update();
}

clz::GraphicsElement *clz::GraphicsElement::get_parent() const
{
    return clz::ElementContainer::instance().get_element(m_parent_id);
}

void clz::GraphicsElement::set_layer(clz::GraphicsLayer *layer)
{
    if(!layer) return;
    for(auto child_id : m_childrens){
        auto child = clz::ElementContainer::instance().get_element(child_id);
        if(!child) continue;
        child->set_layer(layer);
    }
    m_layer_id = layer->get_element_id();
}

clz::GraphicsLayer *clz::GraphicsElement::get_layer() const
{
    auto layer = clz::ElementContainer::instance().get_element(m_layer_id);
    return dynamic_cast<clz::GraphicsLayer*>(layer);
}

clz::GeoMap *clz::GraphicsElement::get_map() const
{
    auto root = clz::ElementContainer::instance().get_element(clz::root_item);
    if (root != nullptr) {
        return root->get_map();
    }
    return nullptr;
}

void clz::GraphicsElement::add_element(clz::GraphicsElement *element)
{
    if(!element) return;
    element->set_parent(this);
    clz::ElementContainer::instance().add_element(element->get_element_id(), element);
}

void clz::GraphicsElement::remove_element(clz::GraphicsElement *element)
{
    if(!element) return;
    if(element->get_parent() != this) return;
    element->set_selected(false);
    element->set_parent(nullptr);
    clz::ElementContainer::instance().remove_element(element->get_element_id());
}

void clz::GraphicsElement::delete_elements()
{
    set_selected(false);
    clz::ElementContainer::instance().remove_elements(m_childrens.toList());
    m_childrens.clear();
}

int clz::GraphicsElement::count_elements() const
{
    return m_childrens.count();
}

void clz::GraphicsElement::set_element_id(QString id)
{
    if(id == clz::empty){
        id = clz::ElementContainer::instance().generate_uuid();
    } else {
        clz::ElementContainer::instance().update_element(m_element_id, id);
    }
    m_element_id = id;
}

QString clz::GraphicsElement::get_element_id() const
{
    return m_element_id;
}

void clz::GraphicsElement::set_selectable(bool val)
{
    if(m_selectable == val) return;
    m_selectable = val;
    if(!m_selectable){
        set_selected(false);
    }
}

bool clz::GraphicsElement::is_selectable() const
{
    return m_selectable;
}

void clz::GraphicsElement::set_selected(bool val)
{
    if(m_selected == val || !is_selectable()) return;
    m_selected = val;
    // todo 放入选中列表中
}

bool clz::GraphicsElement::is_seleceted() const
{
    return m_selected;
}

void clz::GraphicsElement::set_visible(bool val)
{
    if(m_visible == val) return;
    m_visible = val;
}

bool clz::GraphicsElement::is_visible() const
{
    return m_visible;
}
