#include "ClzVLayout.h"
#include "ClzCore/ClzObjectManager.h"
#include "GeoFunction/GeoFunctionUtility.h"

clz::ClzVLayout::ClzVLayout(clz::ClzLayout *parent) : ClzLayout(parent)
{
    m_sensitive_with_fold = true;
}
// 上下叠放
void clz::ClzVLayout::relayout()
{
//    auto layout_pos = pos();
//    auto layout_size = size();

    QPoint pre_pos;
    QSize pre_size;
    bool pre_folded = false;
    int max_width = 0;
    int total_height = 0;
    auto widgets = childrens();
    for(int i = 0, si = widgets.size(); i < si; i++){
        auto widget = dynamic_cast<ClzWidget*>(
                clz::ClzObjectManager::instance().get_element(widgets[i]));
        if(!widget) continue;
        if(widget->size().width() > max_width){max_width = widget->size().width();}
        total_height += (widget->size().height() + 1);
        if(i == 0){
            // 直接放在pos size上
            widget->move(QPoint(pos().x() + m_margin_left,pos().y() + m_margin_top));
        } else {
            int height = pre_pos.y() + m_margin_top;
            if(pre_folded && m_sensitive_with_fold) {
                height += m_bar_height;
            } else {
                height += pre_size.height();
            }
            auto npos = QPoint(pre_pos.x(), height);
            widget->move(npos);
        }

        pre_pos = widget->pos();
        pre_size = widget->size();
        pre_folded = widget->fold();
        widget->paint();
    }
    resize(QSize(max_width, total_height));
}
