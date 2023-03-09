#include "ProjectionESG3857.h"
#include <qmath.h>

ProjectionESG3857::ProjectionESG3857()
    : Projection("EPSG3857",
                 "WGS84",
                 "QGVProjection used in web mapping applications like "
                 "Google/Bing/OpenStreetMap/etc. Sometimes known as "
                 "EPSG:900913.")
{
    mEarthRadius = 6378137.0; /* meters */
    mOriginShift = 2.0 * M_PI * mEarthRadius / 2.0;
    mMetersPerDegree = 2.0 * M_PI * mEarthRadius / 360.0;
    mRadiansPerDegree = M_PI / 180.0;
    mDegreesPerRadian = 180.0 / M_PI;
    mGeoBoundary = clz::GeoRect(85, -180, -85, +180);
    // 转换为大致的地理范围,单位为m
    mProjBoundary = geoToProj(mGeoBoundary);
}

clz::GeoRect ProjectionESG3857::boundaryGeoRect() const
{
    return mGeoBoundary;
}

QRectF ProjectionESG3857::boundaryProjRect() const
{
    return mProjBoundary;
}

QPointF ProjectionESG3857::geoToProj(const clz::GeoPos &geoPos) const
{
    const double lon = geoPos.longitude();
    const double lat = (geoPos.latitude() > mGeoBoundary.topLeft().latitude()) ? mGeoBoundary.topLeft().latitude()
                                                                               : geoPos.latitude();
    const double x = lon * mOriginShift / 180.0;
    const double preY = -qLn(qTan((90.0 + lat) * M_PI / 360.0)) / (M_PI / 180.0);
    const double y = preY * mOriginShift / 180.0;
    return QPointF(x, y);
}

clz::GeoPos ProjectionESG3857::projToGeo(const QPointF &projPos) const
{
    const double lon = (projPos.x() / mOriginShift) * 180.0;
    const double preLat = (-projPos.y() / mOriginShift) * 180.0;
    const double lat = 180.0 / M_PI * (2.0 * qAtan(qExp(preLat * M_PI / 180.0)) - M_PI / 2.0);
    return clz::GeoPos(lat, lon);
}

QRectF ProjectionESG3857::geoToProj(const clz::GeoRect &geoRect) const
{
    QRectF rect;
    rect.setTopLeft(geoToProj(geoRect.topLeft()));
    rect.setBottomRight(geoToProj(geoRect.bottomRight()));
    return rect;
}

clz::GeoRect ProjectionESG3857::projToGeo(const QRectF &projRect) const
{
    return clz::GeoRect(projToGeo(projRect.topLeft()), projToGeo(projRect.bottomRight()));
}

double ProjectionESG3857::geodesicMeters(const clz::GeoPos &Pos1, const clz::GeoPos &Pos2) const
{
    const double latitudeArc = (Pos1.latitude() - Pos2.latitude()) * M_PI / 180.0;
    const double longitudeArc = (Pos1.longitude() - Pos2.longitude()) * M_PI / 180.0;
    const double latitudeH = qPow(sin(latitudeArc * 0.5), 2);
    const double lontitudeH = qPow(sin(longitudeArc * 0.5), 2);
    const double lonFactor = cos(Pos1.latitude() * M_PI / 180.0) * cos(Pos2.latitude() * M_PI / 180.0);
    const double arcInRadians = 2.0 * asin(sqrt(latitudeH + lonFactor * lontitudeH));
    return mEarthRadius * arcInRadians;
}
