#ifndef CLZHLAYOUT_H
#define CLZHLAYOUT_H

#include "ClzLayout.h"

namespace clz {
class ClzHLayout : public ClzLayout
{
public:
    ClzHLayout(ClzLayout* parent = 0);
    void relayout() override;

private:
    const int m_bar_width = 20;
};
}

#endif // CLZHLAYOUT_H
