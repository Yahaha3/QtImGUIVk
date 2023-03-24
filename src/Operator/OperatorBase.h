#ifndef OPERATORBASE_H
#define OPERATORBASE_H

#include <QObject>
#include "Map/GeoMap.h"
#include "Element/Layer/GraphicsLayer.h"
#include "Element/GraphicsDrawElement.h"
#include "GeoFunction/GeoMapGlobal.h"
#include <QDateTime>

// 用于存储element组合
// 是直接呈现给用户的结构,signature对应的是数据绑定

namespace clz {

class OperatorBase : public QObject
{
    Q_OBJECT
public:
    OperatorBase(GeoMap* map, QString signature, QObject* parent = 0);
    ~OperatorBase();
    GeoMap* map() const;
    bool is_valid() const;

    virtual ClzResult generate_operator(const clz::GeoPos& position, bool noted = true);
    virtual ClzResult generate_operator(const QVector<clz::GeoPos>& vertices, bool noted = true);
    virtual void destroy_operator(bool noted = true);

    virtual ClzResult translate_to(clz::GeoPos position);
    virtual void translate(clz::GeoPos offset);
    virtual void rotate(double angle);

    virtual void update_vertices(QVector<clz::GeoPos>, bool noted = true);
    virtual ClzResult update_vertex(clz::GeoPos, int);
    virtual ClzResult add_vertex(clz::GeoPos, int index);
    virtual ClzResult remove_vertex(int index);
    virtual void append_vertex(clz::GeoPos);

    bool visible() const;
    virtual void set_visible(bool visible);
    QString signature() const;
    void set_icon_template(const QString icon);
    QString icon_template() const;

    void set_layer(const QString& name);
    GraphicsLayer* layer() const;

    GraphicsDrawElement* element_body();
    GraphicsDrawElement* element_center();
    GraphicsDrawElement* element_vertex(int index);

    virtual void refresh();


protected:
    GeoPos m_center;
    QVector<GeoPos> m_vertices;
    bool m_is_valid = true;

    // operator的element组成
    QString m_element_body;
    QString m_element_center;
    QStringList m_element_vertices;
private:
    GeoMap* m_map;
    // 当前这组对象所属的图层
    QString m_layer;

    bool m_visible;
    QString m_signature;
    // 图标模板,可以用于生成顺序图标或直接使用
    QString m_icon_template;
};
}

#endif // OPERATORBASE_H
