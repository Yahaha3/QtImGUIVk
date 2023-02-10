#ifndef GEOFUNCTIONUTILITY_H
#define GEOFUNCTIONUTILITY_H

#include "implot.h"
#include <QVector>
#include "GeoMapGlobal.h"

class Projection;

namespace clz {
class GeoFunctionUtility

{
public:
    GeoFunctionUtility();

    // 瓦片和经纬度的互相转换
    static int long2tilex(double lon, int z);
    static int lat2tiley(double lat, int z);
    static double tilex2long(int x, int z);
    static double tiley2lat(int y, int z);

    // implot转map
    static ImPlotPoint implot2scene(ImPlotPoint pt);
    static ImPlotPoint scene2implot(ImPlotPoint pt);

    // implot位置转像素位置
    static ImPlotPoint implot2px(ImPlotPoint pt);
    static ImPlotPoint px2implot(ImPlotPoint pt);

    static QVector<QPointF> geo_list_to_proj_list(QVector<clz::GeoPos> list, Projection* projection);
    static QVector<clz::GeoPos> proj_list_to_geo_list(QVector<QPointF> list, Projection* projection);

    static QPointF get_polygon_center(QVector<QPointF> vertices);
    static QPointF get_polyline_center(QVector<QPointF> vertices);
};
}

#endif // GEOFUNCTIONUTILITY_H