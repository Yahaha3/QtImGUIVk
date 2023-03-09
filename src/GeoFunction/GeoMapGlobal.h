#ifndef GEOMAPGLOBAL_H
#define GEOMAPGLOBAL_H

#include <iostream>
#include <QObject>
#include "implot.h"

class ImageVk;
namespace clz {

enum ClzResult{
    ClzFail,
    ClzError,
    ClzSuccess,
    ClzNullptr,

    ClzEmptyArray,
    ClzOutOfRange,
    ClzDontContains,

    ClzNotVaild,
    ClzIconError,

    ClzMapError

};


struct TilePos {
    int x, y, z;

    TilePos(){};
    TilePos(int tx, int ty, int tz){
        x = tx;
        y = ty;
        z = tz;
    }
    TilePos(const TilePos& other){
        x = other.x;
        y = other.y;
        z = other.z;
    }
    TilePos(const TilePos&& other){
        x = std::move(other.x);
        y = std::move(other.y);
        z = std::move(other.z);
    }

    QString tozxy() {
        auto info = QString("z:%1 x:%2 y:%3")
                .arg(QString::number(z))
                .arg(QString::number(x))
                .arg(QString::number(y));
        return info;
    }

    TilePos& operator=(const TilePos& other){
        z = other.z;
        x = other.x;
        y = other.y;
        return *this;
    }

    TilePos& operator=(const TilePos&& other){
        z = std::move(other.z);
        x = std::move(other.x);
        y = std::move(other.y);
        return *this;
    }

    bool operator==(const TilePos& other) {
        bool result = false;
        if(other.z == z && other.x == x && other.y == y) result = true;
        return result;
    }

    bool operator<(const TilePos& other) const{
        if(z < other.z){
            return true;
        }
        if(z > other.z){
            return false;
        }
        if(x < other.x){
            return true;
        }
        if(x > other.x){
            return false;
        }
        return y < other.y;
    }

    std::tuple<ImPlotPoint, ImPlotPoint> bounds() const {
        double n = std::pow(2, z);
        double t = 1.0 / n;
        return {
                    {x*t, (1+y)*t},
                    {(1+x)*t, y*t}
               };
    }

    TilePos parent(int zoom, bool& ok) const {
        if(zoom >= z || zoom < 0){
            ok = false;
            return TilePos();
        }
        ok = true;
        const int delta_z = z - zoom;
        const int factor = static_cast<int>(pow(2, delta_z));
        const int px = static_cast<int>(floor(x / factor));
        const int py = static_cast<int>(floor(y / factor));
        return TilePos(px, py, zoom);
    }

    bool contains(const TilePos& other) const{
        if(z >= other.z) return false;
        bool ok = false;
        auto parent = other.parent(z, ok);
        return (x == parent.x && y == parent.y);
    }
};

enum TileState : int {
    Unavailable = 0, // tile not available
    Loaded,          // tile has been loaded into  memory
    Downloading,     // tile is downloading from server
    Ready            // tile is saved to disk, but not loaded into memory
};

typedef ImageVk TileImage;
struct Tile {
    Tile() : state(TileState::Unavailable){  }
    std::shared_ptr<TileImage> image;
    TileState state;
};

class GeoPos
{
public:
    GeoPos();
    GeoPos(double lat, double lon, double alt = 0);
    GeoPos(const GeoPos& other);
    GeoPos(const GeoPos&& other);
    GeoPos(const QPointF& other);
    GeoPos& operator=(const GeoPos& other);
    GeoPos& operator=(const GeoPos&& other);
    bool operator==(const GeoPos& other);

    GeoPos operator-(const GeoPos& other);
    GeoPos operator+(const GeoPos& other);

    double latitude() const{ return mLat; }
    double longitude() const { return mLon; }
    double altitude() const{ return mAlt;}
    bool radius(double& value) const{
        if(mRadius < 0) return false;
        value = mRadius;
        return true;
    }

    bool is_2d() const {return m2d_;}

    void setLat(double lat){
        mLat = qMax(-90.0, lat);
        mLat = qMin(90.0, lat);
    }
    void setLon(double lon){
        if (lon > 180.000001) {
            lon = fmod((180.0 + lon), 360.0) - 180.0;
        } else {
            if (lon < -180.000001) {
                lon = 180.0 - fmod((180.0 - lon), 360.0);
            }
        }
        mLon = lon;
    }
    void setAlt(double alt){
        m2d_ = (alt == 0) ? true : false;
        mAlt = alt;
    }
    void setRadius(double radius){mRadius = radius;}

    bool zero() const;
private:
    double mLat;
    double mLon;
    double mAlt = 0.0;
    double mRadius = -1.0;

    bool m2d_ = true;
};

class GeoRect
{
public:
    GeoRect();
    GeoRect(double lat1, double lon1, double lat2, double lon2);
    GeoRect(GeoPos const& pos1, GeoPos const& pos2);
    GeoRect(const GeoRect& other);
    GeoRect(const GeoRect&& other);
    GeoRect(const QVector<double>& other);
    GeoRect& operator=(const GeoRect& other);
    GeoRect& operator=(const GeoRect&& other);
    bool operator==(const GeoRect& other);

    GeoPos topLeft() const;
    GeoPos bottomRight() const;

    GeoPos center();

private:
    GeoPos mTopLeft;
    GeoPos mBottomRight;
};

class ImPlotPointArray{
public:
    ImPlotPointArray(int size);
    double* x_array();
    double* y_array();
    int size() const;

    void set(const ImPlotPoint& other, int idx);
    ImPlotPoint get(int idx);
private:
    int m_array_size;
    double* m_x;
    double* m_y;
};

enum ItemFlag
{
    IgnoreScale     = 0x01,
    IgnoreAzimuth   = 0x02,
    Highlightable   = 0x03,
    Highlighted     = 0x04,
    HighlightCustom = 0x05,
    SelectCustom    = 0x06,
    Transformed     = 0x07,
    Clickable       = 0x08,
    Outline         = 0x09,
    Hoverable       = 0x0a,
};
Q_DECLARE_FLAGS(ItemFlags, ItemFlag)

}

Q_DECLARE_METATYPE(clz::TilePos)

#endif // GEOMAPGLOBAL_H
