#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <imgui.h>
#include <iostream>
#include "backends/imgui_impl_vulkan.h"
#include <QByteArray>
#include <QImage>

struct ImageVk
{

    ImageVk(): Width(0),
               Height(0),
               ID(0)
    {

    }

    ImageVk(const ImageVk &) = delete;
    ImageVk &operator=(const ImageVk &) = delete;

    ~ImageVk();
    void set_origin_data(std::string data);

    bool loadFromOrigin();
    void updateOrigin(unsigned char* data, int width, int height);

    ImTextureID get_image_texture(bool& ok);

    int Width;
    int Height;

private:
    bool LoadFromBytearry(char* data, int size);
    bool LoadFromBytearry();

private:
    VKuint ID;
    std::string m_origin_chr;
    QImage m_image;
};
