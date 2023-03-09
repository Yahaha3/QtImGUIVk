#include "ClzVLayout.h"
#include "ClzCore/ClzObjectManager.h"

clz::ClzVLayout::ClzVLayout(clz::ClzLayout *parent) : ClzLayout(parent)
{

}
// 上下叠放
void clz::ClzVLayout::relayout()
{
//    auto layout_pos = pos();
//    auto layout_size = size();

    QPoint pre_pos;
    QSize pre_size;
    for(int i = 0, si = m_widgets.size(); i < si; i++){
        auto widget = dynamic_cast<ClzWidget*>(
                clz::ClzObjectManager::instance().get_element(m_widgets[i]));
        if(!widget) continue;
        if(i == 0){
            // 直接放在pos size上
            widget->move(QPoint(m_margin_left, m_margin_top));
        } else {
            auto npos = QPoint(pre_pos.x(), pre_pos.y() + pre_size.height() + m_margin_top);
            widget->move(npos);
        }

        pre_pos = widget->pos();
        pre_size = widget->size();

    }
}
