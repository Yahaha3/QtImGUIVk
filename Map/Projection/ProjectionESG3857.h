#ifndef PROJECTIONESG3857_H
#define PROJECTIONESG3857_H

#include "Projection.h"
#include <QRectF>

class ProjectionESG3857 : public Projection
{
public:
    ProjectionESG3857();
    virtual ~ProjectionESG3857() = default;
private:
    clz::GeoRect boundaryGeoRect() const override final;
    QRectF boundaryProjRect() const override final;

    QPointF geoToProj(clz::GeoPos const& geoPos) const override final;
    clz::GeoPos projToGeo(QPointF const& projPos) const override final;
    QRectF geoToProj(clz::GeoRect const& geoRect) const override final;
    clz::GeoRect projToGeo(QRectF const& projRect) const override final;

    double geodesicMeters(const clz::GeoPos &projPos1, const clz::GeoPos &projPos2) const override;
private:
    double mEarthRadius;
    double mOriginShift;

    double mMetersPerDegree;
    double mRadiansPerDegree;
    double mDegreesPerRadian;

    clz::GeoRect mGeoBoundary;
    QRectF mProjBoundary;
};

#endif // PROJECTIONESG3857_H
