#include "ClzObject.h"
#include "ClzObjectManager.h"

clz::ClzObject::ClzObject(ClzObject *parent) : QObject(parent)
{
    if(parent){
        m_parent = parent->get_object_id();
    }

    set_object_id();
    clz::ClzObjectManager::instance().add_element(get_object_id(), this);
}

clz::ClzObject::~ClzObject()
{
    delete_elements();
    auto parent = clz::ClzObjectManager::instance().get_element(m_parent);
    if(parent != nullptr){
        parent->m_childrens.remove(get_object_id());
    }
    clz::ClzObjectManager::instance().remove_element(get_object_id());
}

void clz::ClzObject::set_parent(clz::ClzObject *obj)
{
    if(obj && obj->get_object_id() == m_parent) return;
    auto parent_bef = clz::ClzObjectManager::instance().get_element(m_parent);
    if(parent_bef != nullptr){
        parent_bef->m_childrens.remove(get_object_id());
    }

    m_parent = obj->get_object_id();
    obj->m_childrens.insert(get_object_id());
}

clz::ClzObject *clz::ClzObject::parent() const
{
    return clz::ClzObjectManager::instance().get_element(m_parent);
}

void clz::ClzObject::add_child(clz::ClzObject *obj)
{
    if(!obj) return;
    obj->set_parent(this);
    clz::ClzObjectManager::instance().add_element(obj->get_object_id(), obj);
}

void clz::ClzObject::remove_child(clz::ClzObject *obj)
{
    if(!obj) return;
    if(obj->parent() != this) return;
    obj->set_parent(nullptr);
    clz::ClzObjectManager::instance().remove_element(obj->get_object_id());
}

void clz::ClzObject::delete_elements()
{
    clz::ClzObjectManager::instance().remove_elements(m_childrens.toList());
    m_childrens.clear();
}

int clz::ClzObject::count_childrens() const
{
    return m_childrens.count();
}

void clz::ClzObject::set_object_id(QString id)
{
    if(id == clz::empty){
        id = clz::ClzObjectManager::instance().generate_uuid();
    } else {
        clz::ClzObjectManager::instance().update_element(m_object_id, id);
    }
    m_object_id = id;
}

QString clz::ClzObject::get_object_id() const
{
    return m_object_id;
}
