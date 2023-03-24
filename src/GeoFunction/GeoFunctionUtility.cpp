#include "GeoFunctionUtility.h"
#include <qmath.h>
#include "Map/Projection/Projection.h"
#include <QFile>

#define PI 3.14159265359

clz::GeoFunctionUtility::GeoFunctionUtility()
{

}

int clz::GeoFunctionUtility::long2tilex(double lon, int z)
{
    return (int)(floor((lon + 180.0) / 360.0 * (1 << z)));
}

int clz::GeoFunctionUtility::lat2tiley(double lat, int z)
{
    double latrad = lat * PI/180.0;
    return (int)(floor((1.0 - asinh(tan(latrad)) / PI) / 2.0 * (1 << z)));
}

double clz::GeoFunctionUtility::tilex2long(int x, int z)
{
    return x / (double)(1 << z) * 360.0 - 180;
}

double clz::GeoFunctionUtility::tiley2lat(int y, int z)
{
    double n = PI - 2.0 * PI * y / (double)(1 << z);
    return 180.0 / PI * atan(0.5 * (exp(n) - exp(-n)));
}
// 此处操作相当于移动中心点
ImPlotPoint clz::GeoFunctionUtility::implot2scene(ImPlotPoint pt)
{
    auto nx = pt.x - 0.5;
    auto ny = pt.y - 0.5;
    return ImPlotPoint(nx, ny);
}

ImPlotPoint clz::GeoFunctionUtility::scene2implot(ImPlotPoint pt)
{
    auto nx = pt.x + 0.5;
    auto ny = pt.y + 0.5;
    return ImPlotPoint(nx, ny);
}

ImPlotPoint clz::GeoFunctionUtility::implot2px(ImPlotPoint pt)
{
    auto pos = ImPlot::GetPlotPos();
    auto size = ImPlot::GetPlotSize();
    auto limit = ImPlot::GetPlotLimits();

    auto ratio_x = (pt.x - limit.X.Min) / limit.X.Size();
    auto ratio_y = (pt.y - limit.Y.Min) / limit.Y.Size();

    auto offset_x = size.x * ratio_x;
    auto offset_y = size.y * ratio_y;

    return ImPlotPoint(pos.x + offset_x, pos.y + offset_y);
}

ImPlotPoint clz::GeoFunctionUtility::px2implot(ImPlotPoint pt)
{
    auto pos = ImPlot::GetPlotPos();
    auto size = ImPlot::GetPlotSize();
    auto limit = ImPlot::GetPlotLimits();

    auto plot_px = ImPlotPoint(pt.x - pos.x, pt.y - pos.y);
    auto ratio_x = plot_px.x / size.x;
    auto ratio_y = plot_px.y / size.y;

    auto offset_x = limit.X.Size() * ratio_x;
    auto offset_y = limit.Y.Size() * ratio_y;

    return ImPlotPoint(limit.X.Min + offset_x, limit.Y.Min + offset_y);
}

QVector<QPointF> clz::GeoFunctionUtility::geo_list_to_proj_list(QVector<clz::GeoPos> list, Projection *projection)
{
    QVector<QPointF> ret;
    if(!projection) return ret;
    for(auto child: list)
    {
        auto proj = projection->geoToProj(child);
        ret.append(proj);
    }
    return ret;
}

QVector<clz::GeoPos> clz::GeoFunctionUtility::proj_list_to_geo_list(QVector<QPointF> list, Projection *projection)
{
    QVector<clz::GeoPos> ret;
    if(!projection) return ret;
    for(auto child: list)
    {
        auto proj = projection->projToGeo(child);
        ret.append(proj);
    }
    return ret;
}

QPointF clz::GeoFunctionUtility::get_polygon_center(QVector<QPointF> vertices)
{
    //计算多边形形心
    if(vertices.size() < 1) return QPointF();
    if(vertices.size() < 2) return vertices.first();
    double cx = 0,cy = 0;
    double A = 0;
    vertices.append(vertices[0]);//首尾相连
    for(int i = 0; i < vertices.size() - 1; i++)
    {
        auto pti = vertices[i];
        auto ptip = vertices[i + 1];
        cx += ((pti.x() + ptip.x()) * (pti.x() * ptip.y() - ptip.x() * pti.y()));
        cy += ((pti.y() + ptip.y()) * (pti.x() * ptip.y() - ptip.x() * pti.y()));
        A += (pti.x() * ptip.y() - ptip.x() * pti.y());
    }
    A = A / 2;
    cx = cx / (6 * A);
    cy = cy / (6 * A);
    return QPointF(cx, cy);
}

QPointF clz::GeoFunctionUtility::get_polyline_center(QVector<QPointF> vertices)
{
    if(vertices.size() <= 0) return QPointF();
    double tx = vertices.front().x(), ty = vertices.front().y();
    if(vertices.size() > 1)
    {
        for(int i = 1; i < vertices.size(); i++)
        {
            tx += vertices.at(i).x();
            ty += vertices.at(i).y();
        }
    }
    return QPointF(tx / vertices.size(), ty/ vertices.size());
}

QByteArray clz::GeoFunctionUtility::get_image_bytearray(const QString &path, bool &ok)
{
    ok = false;
    if(!QFile::exists(path)) return QByteArray();
    QFile file(path);
    if(file.open(QFile::ReadOnly)){
        ok = true;
        auto data = file.readAll();
        return data;
    }
    return QByteArray();
}

//所有内核相关的业务都放这里来转吧
#include "AosKernelCommon.h"
#include "algorithm_utilities.h"

QString clz::GeoFunctionUtility::kernel_apps_dir()
{
    return aos::AosKernelCommon::get_apps_dir();
}

clz::GeoPos clz::GeoFunctionUtility::get_target_lonlat_with_distance(const clz::GeoPos &p0, const clz::GeoPos &p1, double distance)
{
    double target_lon, target_lat;
    aos::algo_utils::lonlat_distancing(p1.longitude(), p1.latitude(),
                                       p0.longitude(), p0.latitude(),
                                       distance,
                                       target_lon, target_lat);
    return GeoPos(target_lat, target_lon);
}

QString clz::GeoFunctionUtility::cpsgr_icon_path(const QString &icon)
{
    auto pre = aos::AosKernelCommon::get_installer_dir();
    auto path = pre + QString("/UX/csgrs/%1_64x64.png").arg(icon);
    return path;
}
