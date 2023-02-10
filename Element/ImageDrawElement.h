#ifndef IMAGEDRAWELEMENT_H
#define IMAGEDRAWELEMENT_H

#include "GraphicsDrawElement.h"
#include "common/Image.h"

namespace clz {

class ImageDrawElement : public GraphicsDrawElement
{
public:
    ImageDrawElement();
    void element_projection() override;
    QRectF proj_shape() const override;
    void paint() override;

    void load_image(const QByteArray& raw);

private:
    ImageVk m_image;
};

}

#endif // IMAGEDRAWELEMENT_H
