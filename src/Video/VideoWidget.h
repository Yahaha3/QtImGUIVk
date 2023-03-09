#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include "ClzWidgets/ClzWidget.h"
#include "Element/ImageDrawElement.h"
#include "VideoDecode.h"

namespace clz {
class VideoWidget : public ClzWidget
{
    Q_OBJECT
public:
    VideoWidget(ClzWidget* parent = 0);

    void init() override;
    void paint() override;

private slots:
    void slot_video_decode_info(uchar* data, int w, int h);

private:
    std::shared_ptr<clz::ImageDrawElement> m_video;
    std::shared_ptr<clz::VideoDecode> m_video_decode;
};
}

#endif // VIDEOWIDGET_H
