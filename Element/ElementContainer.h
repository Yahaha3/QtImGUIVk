#ifndef ELEMENTCONTAINER_H
#define ELEMENTCONTAINER_H

#include <QHash>
#include "GraphicsElement.h"

namespace clz {

class ElementContainer
{
public:
    static ElementContainer& instance();

    QString generate_uuid();

    GraphicsElement* get_element(const QString& id);
    QList<GraphicsElement*> get_elements(const QStringList& list);
    void add_element(const QString& id, GraphicsElement* element);
    void remove_element(const QString& id);
    void remove_elements(const QStringList& list);
    bool update_element(const QString& id_before, const QString& id_after);
    int element_count() const;
    void clear_elements();

    bool contains_element(const QString& id);

private:
    ElementContainer();

private:
    static QHash<QString, GraphicsElement*> m_element_container;
};

}


#endif // ELEMENTCONTAINER_H
