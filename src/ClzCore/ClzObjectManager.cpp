#include "ClzObjectManager.h"
#include "common/ManagerTemplate.h"

ManagerTemplate<clz::ClzObject> &clz::ClzObjectManager::instance()
{
    static ManagerTemplate<clz::ClzObject> p;
    return p;
}
