#ifndef GRAPHICSLAYER_H
#define GRAPHICSLAYER_H

#include "Element/GraphicsElement.h"

namespace clz {

class GraphicsLayer: public GraphicsElement
{
    Q_OBJECT
public:
    GraphicsLayer(const QString& name, const QString& description);

    void set_name(const QString& name);
    QString get_name() const;

    void set_description(const QString& description);
    QString get_description() const;

private:
    // 图层名称
    QString m_name;
    // 图层描述
    QString m_desccription;
};

}

#endif // GRAPHICSLAYER_H
