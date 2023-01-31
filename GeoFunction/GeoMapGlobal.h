#ifndef GEOMAPGLOBAL_H
#define GEOMAPGLOBAL_H

#include <iostream>
#include <QObject>

class ImageVk;
namespace clz {

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
        bool result = false;
        if(z < other.z || x < other.x || y < other.y){
            result = true;
        }
        return result;
    }
};

enum TileState : int {
    Unavailable = 0, // tile not available
    Loaded,          // tile has been loaded into  memory
    Downloading,     // tile is downloading from server
    OnDisk           // tile is saved to disk, but not loaded into memory
};

typedef ImageVk TileImage;
struct Tile {
    Tile() : state(TileState::Unavailable){  }
    std::shared_ptr<TileImage> image;
    TileState state;
};

}

Q_DECLARE_METATYPE(clz::TilePos)

#endif // GEOMAPGLOBAL_H
