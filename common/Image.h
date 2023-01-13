#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <iostream>
#include <stb/stb_image.h>
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

    ~ImageVk()
    {
        if (ID != VK_NULL_HANDLE)
            ImGui_ImplVulkan_DeleteTexture(ID);
    }

    bool LoadFromBytearry(char* data, int size){
        if (ID != VK_NULL_HANDLE)
            ImGui_ImplVulkan_DeleteTexture(ID);
        ID = 0;
        int n = 0;
        unsigned char *image_data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(data), size,
                                                          &Width, &Height, &n, 4);
//        auto image_data = reinterpret_cast<unsigned char*>(data);
        if (image_data == nullptr)
            return false;
        ID = ImGui_ImplVulkan_CreateTexture(image_data, Width, Height);
        stbi_image_free(image_data);
        return true;
    }

    void set_origin_data(std::string data){
        origin_ = data;
    }

    bool loadFromOrigin(){
        return LoadFromBytearry(origin_.data(), origin_.size());
    }

    ImTextureID get_image_texture(bool& ok){
        VkResult err;
        ImTextureID id = ImGui_ImplVulkan_GetImTextureID(ID, err);
        ok = true;
        if(err != VK_SUCCESS) ok = false;
        return id;
    }

    int Width;
    int Height;

private:
    VKuint ID;
    std::string origin_;
};
