#include "Image.h"
#include <stb/stb_image.h>
#include <vulkan/vulkan.h>

ImageVk::~ImageVk()
{
    if (ID != VK_NULL_HANDLE)
        ImGui_ImplVulkan_DeleteTexture(ID);
}

bool ImageVk::LoadFromBytearry(char *data, int size)
{
    if (ID != VK_NULL_HANDLE)
        ImGui_ImplVulkan_DeleteTexture(ID);
    ID = 0;
    int n = 0;
    unsigned char *image_data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(data), size,
                                                      &Width, &Height, &n, 4);
    if (image_data == nullptr)
        return false;
    ID = ImGui_ImplVulkan_CreateTexture(image_data, Width, Height);
    stbi_image_free(image_data);
    return true;
}

bool ImageVk::LoadFromBytearry()
{
    auto image_data = m_image.bits();
    if (image_data == nullptr)
        return false;
    ID = ImGui_ImplVulkan_CreateTexture(image_data, Width, Height);
    return true;
}

void ImageVk::set_origin_data(std::string data)
{
    m_origin_chr = data;
}

bool ImageVk::loadFromOrigin()
{
    return LoadFromBytearry(m_origin_chr.data(), m_origin_chr.size());
}

void ImageVk::updateOrigin(unsigned char *data, int width, int height)
{
    if(data == nullptr) return;
    if(width != Width || height != Height){
        Width = width;
        Height = height;
        ID = ImGui_ImplVulkan_CreateTexture(data, Width, Height);
    } else {
        ImGui_ImplVulkan_UpdateTexture(ID, data);
    }
}

ImTextureID ImageVk::get_image_texture(bool &ok)
{
    VkResult err;
    ImTextureID id = ImGui_ImplVulkan_GetImTextureID(ID, err);
    ok = true;
    if(err != VK_SUCCESS) ok = false;
    return id;
}
