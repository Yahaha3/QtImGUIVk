#ifndef GRAPHICSDRAWELEMENT_H
#define GRAPHICSDRAWELEMENT_H

#include "GraphicsElement.h"
#include "Map/Projection/Projection.h"
#include <QRectF>
#include <QVector>
#include <QColor>

namespace clz {

class GraphicsDrawElement: public GraphicsElement
{
    Q_OBJECT
public:
    GraphicsDrawElement();

    void set_flags(QSet<clz::ItemFlag> flags);
    void set_flag(clz::ItemFlag flag, bool enable = true);
    QSet<clz::ItemFlag> get_flags() const;
    bool is_flag(clz::ItemFlag flag) const;

    void refresh();
    void repaint();
    virtual void paint() = 0;

    clz::GeoPos center();
    void set_center(const clz::GeoPos& position);

    void set_vertices(const QVector<clz::GeoPos>& vertices);
    QVector<clz::GeoPos> get_vertices() const;

    void set_back_color(const QColor& color);
    void set_front_color(const QColor& color);
    ImVec4 get_back_color() const;
    ImVec4 get_front_color() const;

protected:
    virtual void element_projection() = 0;
    virtual QRectF proj_shape() const = 0;

protected:
    // 地理坐标,投影坐标,plot坐标
    QVector<clz::GeoPos> m_vertices;
    QVector<QPointF> m_proj_vertices;
    ImPlotPointArray* m_plot_vertices;

    QSet<clz::ItemFlag> m_flags;
    clz::GeoPos m_center;

    QColor m_back_color;
    QColor m_front_color;
};

}

#endif // GRAPHICSDRAWELEMENT_H
