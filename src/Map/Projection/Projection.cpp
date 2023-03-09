#include "Projection.h"
#include <math.h>

Projection::Projection(const QString &id, const QString &name, const QString &description)
    : mID(id)
    , mName(name)
    , mDescription(description)
{

}

QString Projection::getID() const
{
    return mID;
}

QString Projection::getName() const
{
    return mName;
}

QString Projection::getDescription() const
{
    return mDescription;
}

QPointF Projection::sceneToMap(const QPointF &pt)
{   // -0.5 ~~ +0.5
    auto boundary = boundaryProjRect();
    double px = 0, py = 0;
    if(abs(pt.x()) <= 0.5){
        auto width = boundary.width();
        px = width * pt.x();
    }
    if(abs(pt.y()) <= 0.5){
        auto height = boundary.height();
        py = height * pt.y();
    }
    return QPointF(px, py);
}

QPointF Projection::mapToScene(const QPointF &pt)
{
    auto boundary = boundaryProjRect();
    double px = 0, py = 0;
    {
        auto width = boundary.width();
        px = pt.x() / width;
    }
    {
        auto height = boundary.height();
        py = pt.y() / height;
    }
    return QPointF(px, py);
}
