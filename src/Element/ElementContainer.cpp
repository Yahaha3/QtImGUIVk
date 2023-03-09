#include "ElementContainer.h"
//#include <iostream>
//#include <QUuid>

//QHash<QString, clz::GraphicsElement*> clz::ElementContainer::m_element_container;

//clz::ElementContainer &clz::ElementContainer::instance()
//{
//    static clz::ElementContainer p;
//    return p;
//}

//QString clz::ElementContainer::generate_uuid()
//{
//    return QUuid::createUuid().toString();
//}

//clz::GraphicsElement *clz::ElementContainer::get_element(const QString &id)
//{
//    if(contains_element(id)){
//        return m_element_container.value(id);
//    }
//    return nullptr;
//}

//QList<clz::GraphicsElement *> clz::ElementContainer::get_elements(const QStringList &list)
//{
//    QList<clz::GraphicsElement*> result;
//    for(auto id: list){
//        result.append(get_element(id));
//    }
//    return result;
//}

//void clz::ElementContainer::add_element(const QString &id, clz::GraphicsElement *element)
//{
//    if(contains_element(id) && m_element_container[id] != element){
//        m_element_container[id]->delete_elements();
//        m_element_container[id]->deleteLater();
//        m_element_container[id] = nullptr;
//    }
//    m_element_container[id] = element;
//}

//void clz::ElementContainer::remove_element(const QString &id)
//{
//    if(contains_element(id)){
//        m_element_container[id]->delete_elements();
//        m_element_container[id]->deleteLater();
//        m_element_container[id] = nullptr;
//        m_element_container.remove(id);
//    }
//}

//void clz::ElementContainer::remove_elements(const QStringList &list)
//{
//    for(auto id: list){
//        remove_element(id);
//    }
//}

//bool clz::ElementContainer::update_element(const QString &id_before, const QString &id_after)
//{
//    if(contains_element(id_before)){
//        m_element_container[id_after] = m_element_container[id_before];
//        m_element_container[id_before] = nullptr;
//        m_element_container.remove(id_before);
//        return true;
//    }
//    return false;
//}

//int clz::ElementContainer::element_count() const
//{
//    return m_element_container.count();
//}

//void clz::ElementContainer::clear_elements()
//{
//    auto copy = m_element_container;
//    qDeleteAll(copy.begin(), copy.end());
//    m_element_container.clear();
//}

//bool clz::ElementContainer::contains_element(const QString &id)
//{
//    return m_element_container.contains(id);
//}

//clz::ElementContainer::ElementContainer()
//{

//}

ManagerTemplate<clz::GraphicsElement> &clz::ElementContainer::instance()
{
    static ManagerTemplate<clz::GraphicsElement> p;
    return p;
}
