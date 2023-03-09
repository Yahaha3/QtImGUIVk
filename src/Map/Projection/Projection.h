#ifndef PROJECTION_H
#define PROJECTION_H

#include "GeoFunction/GeoMapGlobal.h"
#include <QPointF>
#include <QRectF>

class Projection
{
public:
    Projection(const QString& id, const QString& name, const QString& description);
    virtual ~Projection() = default;

    QString getID() const;
    QString getName() const;
    QString getDescription() const;

    virtual clz::GeoRect boundaryGeoRect() const = 0;
    virtual QRectF boundaryProjRect() const = 0;

    virtual QPointF geoToProj(clz::GeoPos const& geoPos) const = 0;
    virtual clz::GeoPos projToGeo(QPointF const& projPos) const = 0;
    virtual QRectF geoToProj(clz::GeoRect const& geoRect) const = 0;
    virtual clz::GeoRect projToGeo(QRectF const& projRect) const = 0;

    virtual double geodesicMeters(clz::GeoPos const& projPos1, clz::GeoPos const& projPos2) const = 0;

    QPointF sceneToMap(const QPointF& pt);
    QPointF mapToScene(const QPointF& pt);
private:
    Q_DISABLE_COPY(Projection)
    QString mID;
    QString mName;
    QString mDescription;
};

#endif // PROJECTION_H
