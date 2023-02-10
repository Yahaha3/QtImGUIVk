#ifndef MAPCOPYRIGHTITEM_H
#define MAPCOPYRIGHTITEM_H
#include "MapItem.h"
namespace clz {

class MapCopyrightItem: public MapItem
{
public:
    MapCopyrightItem();
    void paint() override;
};

}


#endif // MAPCOPYRIGHTITEM_H
