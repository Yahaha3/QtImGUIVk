#ifndef CLZLAYOUT_H
#define CLZLAYOUT_H

#include <QObject>
#include "ClzCore/ClzObject.h"
#include "ClzWidget.h"
/*
 * 用于实现简单的网格布局
 * 自动铺满,根据当前内容停靠
 * 一个layout相当于一个大的imgui
*/
namespace clz {

class ClzLayout : public ClzObject
{
    Q_OBJECT
public:
    ClzLayout(ClzObject* parent = 0);

    void set_contents_margins(int left, int top, int right, int bottom);

    void add_widget(ClzWidget* widget);
    void remove_widget(ClzWidget* widget);
    bool contain_widget(ClzWidget* widget);

    // 暂时不让调整顺序吧
    // todo: readjust order
    virtual void relayout() = 0;

    QPoint pos();
    QSize size();

    void move(QPoint position);
    void resize(QSize size);

protected:
    int m_margin_left,m_margin_right,m_margin_top,m_margin_bottom;

    QStringList m_widgets;
    QPoint m_position;
    QSize m_size;
};

}

#endif // CLZLAYOUT_H
