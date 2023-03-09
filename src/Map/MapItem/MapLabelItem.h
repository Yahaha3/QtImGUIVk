#ifndef MAPLABELITEM_H
#define MAPLABELITEM_H
#include "MapItem.h"
namespace clz {
class MapLabelItem: public MapItem
{
public:
    MapLabelItem();
    void paint() override;
};
}


#endif // MAPLABELITEM_H
