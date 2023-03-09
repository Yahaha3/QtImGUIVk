#ifndef CLZVLAYOUT_H
#define CLZVLAYOUT_H

#include "ClzLayout.h"

namespace clz {
class ClzVLayout : public ClzLayout
{
public:
    ClzVLayout(ClzLayout* parent = 0);

    void relayout() override;
};
}

#endif // CLZVLAYOUT_H
