#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <imgui.h>
#include <iostream>
#include "backends/imgui_impl_vulkan.h"

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

    bool LoadFromBytearry(char* data, int size);

    void set_origin_data(std::string data);

    bool loadFromOrigin();

    ImTextureID get_image_texture(bool& ok);

    int Width;
    int Height;

private:
    VKuint ID;
    std::string origin_;
};
