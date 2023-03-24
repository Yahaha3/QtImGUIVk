#include "ClzHLayout.h"
#include "ClzCore/ClzObjectManager.h"

clz::ClzHLayout::ClzHLayout(clz::ClzLayout *parent) : ClzLayout(parent)
{
    set_sensitive_width_fold(false);
}

// 左右叠放
void clz::ClzHLayout::relayout()
{
    QPoint pre_pos;
    QSize pre_size;
    bool pre_folded = false; // 左右叠放暂时不允许折叠效果,所以始终保持false
    int max_height = 0;
    int total_width = 0;
    auto widgets = childrens();
    for(int i = 0, si = widgets.size(); i < si; i++){
        auto widget = dynamic_cast<ClzWidget*>(
                clz::ClzObjectManager::instance().get_element(widgets[i]));
        if(!widget) continue;
        if(widget->size().height() > max_height){max_height = widget->size().height();}
        total_width += (widget->size().width() + 2);
        if(i == 0){
            widget->move(QPoint(pos().x() + m_margin_left,pos().y() + m_margin_top));
        } else {
            int width = pre_pos.x() + m_margin_left;
            if(!pre_folded && m_sensitive_with_fold){
                width += m_bar_width;
            } else {
                width += pre_size.width();
            }

            auto npos = QPoint(width, pre_pos.y());
            widget->move(npos);
        }

        pre_pos = widget->pos();
        pre_size = widget->size();
        widget->paint();
    }

    resize(QSize(total_width, max_height));
}
