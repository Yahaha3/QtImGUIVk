#include "GeoMapGlobal.h"
#include "QPointF"
#include <QVector>

clz::GeoPos::GeoPos()
    : mLat(0)
    , mLon(0)
{

}

clz::GeoPos::GeoPos(double lat, double lon, double alt)
{
    setLat(lat);
    setLon(lon);
    setAlt(alt);
}

clz::GeoPos::GeoPos(const clz::GeoPos &other)
    : mLat(other.latitude())
    , mLon(other.longitude())
{
    if(!other.is_2d()){
        setAlt(other.altitude());
    }
    setRadius(other.mRadius);
}

clz::GeoPos::GeoPos(const clz::GeoPos &&other)
    : mLat(std::move(other.latitude()))
    , mLon(std::move(other.longitude()))
{
    if(!other.is_2d()){
        setAlt(other.altitude());
    }
    setRadius(other.mRadius);
}

clz::GeoPos::GeoPos(const QPointF &other)
    : mLat(other.y())
    , mLon(other.x())
{

}

clz::GeoPos &clz::GeoPos::operator=(const clz::GeoPos &other)
{
    mLat = other.latitude();
    mLon = other.longitude();
    if(!other.is_2d()){
        setAlt(other.altitude());
    }
    if(other.mRadius != -1){
        setRadius(other.mRadius);
    }
    return *this;
}

clz::GeoPos &clz::GeoPos::operator=(const clz::GeoPos &&other)
{
    mLat = std::move(other.latitude());
    mLon = std::move(other.longitude());
    if(!other.is_2d()){
        setAlt(other.altitude());
    }
    setRadius(other.mRadius);
    return *this;
}

bool clz::GeoPos::operator==(const clz::GeoPos &other)
{
    bool result = false;
    if(mLat == other.mLat && mLon == other.mLon)
    {
        result = true;
    }

    if(result){
        result = abs(this->altitude() - other.altitude()) > 1e-2 ? false : true;
    }
    return result;
}

clz::GeoPos clz::GeoPos::operator-(const clz::GeoPos &other)
{
    auto lon = mLon - other.longitude();
    auto lat = mLat + other.latitude();
    return GeoPos(lat, lon, other.altitude());
}

clz::GeoPos clz::GeoPos::operator+(const clz::GeoPos &other)
{
    auto lon = mLon + other.longitude();
    auto lat = mLat - other.latitude();
    return GeoPos(lat, lon, other.altitude());
}

bool clz::GeoPos::zero() const
{
    return longitude() < 1E-7 && latitude() < 1E-7;
}

clz::GeoRect::GeoRect()
{

}

clz::GeoRect::GeoRect(double lat1, double lon1, double lat2, double lon2)
{
    mTopLeft = GeoPos(qMax(lat1, lat2), qMin(lon1, lon2));
    mBottomRight = GeoPos(qMin(lat1, lat2), qMax(lon1, lon2));
}

clz::GeoRect::GeoRect(const clz::GeoPos &pos1, const clz::GeoPos &pos2)
{
    mTopLeft = GeoPos(qMax(pos1.latitude(), pos2.latitude()), qMin(pos1.longitude(), pos2.longitude()));
    mBottomRight = GeoPos(qMin(pos1.latitude(), pos2.latitude()), qMax(pos1.longitude(), pos2.longitude()));
}

clz::GeoRect::GeoRect(const clz::GeoRect &other)
    : mTopLeft(other.mTopLeft)
    , mBottomRight(other.mBottomRight)
{
}

clz::GeoRect::GeoRect(const clz::GeoRect &&other)
    : mTopLeft(std::move(other.mTopLeft))
    , mBottomRight(std::move(other.mBottomRight))
{
}

clz::GeoRect::GeoRect(const QVector<double> &other)
{
    if(other.size() == 4){
        mTopLeft = (GeoPos(other.at(1), other.at(0)));
        mBottomRight = (GeoPos(other.at(3), other.at(2)));
    }
}

clz::GeoRect &clz::GeoRect::operator=(const clz::GeoRect &other)
{
    mTopLeft = other.mTopLeft;
    mBottomRight = other.mBottomRight;
    return *this;
}

clz::GeoRect &clz::GeoRect::operator=(const clz::GeoRect &&other)
{
    mTopLeft = std::move(other.mTopLeft);
    mBottomRight = std::move(other.mBottomRight);
    return *this;
}

bool clz::GeoRect::operator==(const clz::GeoRect &other)
{
    if(mTopLeft == other.mTopLeft &&
       mBottomRight == other.mBottomRight)
    {
        return true;
    }
    return false;
}

clz::GeoPos clz::GeoRect::topLeft() const
{
    return mTopLeft;
}

clz::GeoPos clz::GeoRect::bottomRight() const
{
    return mBottomRight;
}

clz::GeoPos clz::GeoRect::center()
{
    auto complex_lon = mBottomRight.longitude() - mTopLeft.longitude();
    auto complex_lat = mTopLeft.latitude() - mBottomRight.latitude();
    return GeoPos(mTopLeft.latitude() - complex_lat/2, mTopLeft.longitude() + complex_lon/2);
}

clz::ImPlotPointArray::ImPlotPointArray(int size)
{
    m_array_size = size;
    m_x = new double[size];
    m_y = new double[size];
}

double *clz::ImPlotPointArray::x_array()
{
    return m_x;
}

double *clz::ImPlotPointArray::y_array()
{
    return m_y;
}

int clz::ImPlotPointArray::size() const
{
    return m_array_size;
}

void clz::ImPlotPointArray::set(const ImPlotPoint &other, int idx)
{
    if(idx < size()){
        m_x[idx] = other.x;
        m_y[idx] = other.y;
    }
}

ImPlotPoint clz::ImPlotPointArray::get(int idx)
{
    ImPlotPoint result;
    if(idx < size()){
        result.x = m_x[idx];
        result.y = m_y[idx];
    }
    return result;
}
