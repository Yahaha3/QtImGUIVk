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

    // 文件操作
    static QByteArray get_image_bytearray(const QString& path, bool& ok);

    // 内核工具
    static QString kernel_apps_dir();
    static GeoPos get_target_lonlat_with_distance(const GeoPos& p0, const GeoPos& p1, double distance);
    static QString cpsgr_icon_path(const QString& icon);
    static QString parse_format(const QJsonValue& value, const QString& format, bool& ok);
    static QString parse_enum(const QJsonValue& value, const QJsonArray& option, bool& ok);

    // 其他常用工具
    template <typename TSet, typename TFlag>
    static inline bool has_flag(TSet set, TFlag flag) { return (set & flag) == flag; }
};
}

#endif // GEOFUNCTIONUTILITY_H
