#include "ImageDrawElement.h"


clz::ImageDrawElement::ImageDrawElement()
{

}

void clz::ImageDrawElement::element_projection()
{

}

QRectF clz::ImageDrawElement::proj_shape() const
{
    return QRectF();
}

void clz::ImageDrawElement::paint()
{
    bool ok = false;
    auto ID = m_image.get_image_texture(ok);
    if(!ok) return;
//    ImPlot::PlotImage(clz::empty.toStdString(), ID,)
}

void clz::ImageDrawElement::load_image(const QByteArray &raw)
{
    m_image.set_origin_data(raw.toStdString());
    m_image.loadFromOrigin();
}
