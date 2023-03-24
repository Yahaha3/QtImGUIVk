#ifndef CLZCLICKABLEFRAME_H
#define CLZCLICKABLEFRAME_H

#include <QObject>
#include "ClzWidgets/ClzWidget.h"
#include "Element/ImageDrawElement.h"

namespace clz {
// 可左右点击的面板
class ClzClickableFrame : public ClzWidget
{
    Q_OBJECT
public:
    ClzClickableFrame(ClzWidget* parent = 0);

    void init() override;
    void paint() override;

    void update_left_info(const QString& left);
    void update_right_info(const QString& right);
    void set_background_image(const QString& path);

signals:
    void sig_clickable_frame_clicked(bool left);

private:
    int check_current_mouse_position(const ImVec2& position);

private:
    std::shared_ptr<clz::ImageDrawElement> m_frame_image;
    QString m_info_left;
    QString m_info_right;
    QString m_image_path;
};

}

#endif // CLZCLICKABLEFRAME_H
