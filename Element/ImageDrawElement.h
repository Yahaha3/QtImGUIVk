#ifndef IMAGEDRAWELEMENT_H
#define IMAGEDRAWELEMENT_H

#include "GraphicsDrawElement.h"
#include "common/Image.h"

namespace clz {

class ImageDrawElement : public GraphicsDrawElement
{
public:
    ImageDrawElement();
    void paint() override;

    void load_image(const QByteArray& raw);
    void set_geometry(const clz::GeoRect& rect);
    void set_geometry(const clz::GeoPos& position,const QSize& size,
                      const QPoint anchor = QPoint(),
                      const float& angle = 0);
    void set_geometry(const clz::GeoPos& position, const float& angle);

private:
    void calculate_geometry();
    void element_projection() override;
    QRectF proj_shape() const override;

private:
    enum class GeometryType
    {
        None,
        ByRect,
        ByPos,
    } m_geometry_type;

    ImageVk m_image;
    float m_image_angle = 0;
    // 矩形图片的地理边框 (ByRect)
    clz::GeoRect m_geo_rect;
    // 图片占用的像素大小
    QSize m_image_size;
    // 图片停靠的相对位置,(ByPos)
    QPointF m_image_anchor;
    // 图片最终在implot中的位置
    ImPlotPoint mbmin, mbmax;
};

}

#endif // IMAGEDRAWELEMENT_H
