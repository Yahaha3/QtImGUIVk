#ifndef MANAGERTEMPLATE_H
#define MANAGERTEMPLATE_H

#include <QObject>
#include <QHash>
#include <QUuid>

template<class ManagerName>
class ManagerTemplate
{
public:
    ManagerTemplate(){};
    QString generate_uuid(){
        return QUuid::createUuid().toString();
    };

    ManagerName* get_element(const QString& id){
        if(contains_element(id)){
            return m_element_container.value(id);
        }
        return nullptr;
    };

    QList<ManagerName*> get_elements(const QStringList& list){
        QList<ManagerName*> result;
        for(auto id: list){
            result.append(get_element(id));
        }
        return result;
    };

    void add_element(const QString& id, ManagerName* element){
        if(contains_element(id) && m_element_container[id] != element){
            m_element_container[id]->delete_elements();
            m_element_container[id]->deleteLater();
            m_element_container[id] = nullptr;
        }
        m_element_container[id] = element;
    };

    void remove_element(const QString& id){
        if(contains_element(id)){
            m_element_container[id]->delete_elements();
            m_element_container[id]->deleteLater();
            m_element_container[id] = nullptr;
            m_element_container.remove(id);
        }
    };

    void remove_elements(const QStringList& list){
        for(auto id: list){
            remove_element(id);
        }
    };
    bool update_element(const QString& id_before, const QString& id_after){
        if(contains_element(id_before)){
            m_element_container[id_after] = m_element_container[id_before];
            m_element_container[id_before] = nullptr;
            m_element_container.remove(id_before);
            return true;
        }
        return false;
    };
    int element_count() const{
        return m_element_container.count();
    };
    void clear_elements(){
        auto copy = m_element_container;
        qDeleteAll(copy.begin(), copy.end());
        m_element_container.clear();
    };

    bool contains_element(const QString& id){
        return m_element_container.contains(id);
    };

private:
    QHash<QString, ManagerName*> m_element_container;
};

#endif // MANAGERTEMPLATE_H
