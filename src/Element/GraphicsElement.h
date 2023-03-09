#ifndef GRAPHICSELEMENT_H
#define GRAPHICSELEMENT_H

#include <QObject>
#include <QSet>

#include "common/clz_geomap_global.h"
#include "GeoFunction/GeoFunctionUtility.h"
#include "GeoFunction/GeoMapGlobal.h"

namespace clz {

class GraphicsLayer;
class GeoMap;
class GraphicsElement : public QObject
{
    Q_OBJECT
public:
    GraphicsElement(GraphicsElement* parent = 0);
    ~GraphicsElement();

    void set_parent(GraphicsElement* parent);
    GraphicsElement* get_parent() const;

    void set_layer(GraphicsLayer* layer);
    GraphicsLayer* get_layer() const;

    virtual GeoMap* get_map() const;

    void add_element(GraphicsElement* element);
    void remove_element(GraphicsElement* element);
    void delete_elements();
    int count_elements() const;

    void set_element_id(QString id = clz::empty);
    QString get_element_id() const;

    void set_selectable(bool val);
    bool is_selectable() const;
    void set_selected(bool val);
    bool is_seleceted() const;
    void set_visible(bool val);
    bool is_visible() const;

private:
    QString m_parent_id;
    bool m_visible;
    bool m_selectable;
    bool m_selected;

    QString m_type;
    QSet<QString> m_childrens;
    QString m_element_id;
    QString m_layer_id;
};

}


#endif // GRAPHICSELEMENT_H
