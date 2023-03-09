#ifndef MAPITEM_H
#define MAPITEM_H

#include "implot.h"
#include "Map/GeoMap.h"
#include <QColor>

namespace clz {

class MapItem
{
public:
    MapItem();
    virtual ~MapItem();

    void set_map(GeoMap* geomap);
    std::shared_ptr<GeoMap> get_map() const;

    virtual void paint() = 0;

    void set_back_color(const QColor& color);
    ImVec4 get_back_color();

private:
    std::shared_ptr<GeoMap> m_map;
    QColor m_back_color;
};

}


#endif // MAPITEM_H
