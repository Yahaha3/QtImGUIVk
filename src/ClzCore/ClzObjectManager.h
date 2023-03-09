#ifndef CLZOBJECTMANAGER_H
#define CLZOBJECTMANAGER_H

#include <QHash>
#include "ClzObject.h"
#include "common/ManagerTemplate.h"

namespace clz {
class ClzObjectManager
{
public:
    static ManagerTemplate<clz::ClzObject>& instance();
};
}

#endif // CLZOBJECTMANAGER_H
