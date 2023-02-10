#ifndef MAPSCALEITEM_H
#define MAPSCALEITEM_H

#include "MapItem.h"
#include "GeoFunction/GeoMapGlobal.h"

namespace clz {

class MapScaleItem : public MapItem
{
public:
    MapScaleItem();
    void paint() override;

private:
    void calculate_item();
    QString get_length_meters();

private:
    const int m_default_length_px = 150;
    const int m_min_length_px     = 120;
    std::shared_ptr<ImPlotPointArray> m_scale_pt;
    ImPlotPoint m_label_pos;
    double m_length_meters;
};

}


#endif // MAPSCALEITEM_H
