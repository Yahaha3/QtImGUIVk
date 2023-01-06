#include <QApplication>
#include <QVulkanInstance>
#include <iostream>
#include "common/vulkanwindow.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    std::cout << "clz RUN!" << std::endl;

    QVulkanInstance inst;
    inst.setLayers(QByteArrayList() << "VK_LAYER_LUNARG_standard_validation");

    if (!inst.create())
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());
    VulkanWindow* window = new VulkanWindow;
    window->setVulkanInstance(&inst);

    ImGuiVulkanWidget* widget = new ImGuiVulkanWidget(window);
    widget->resize(1200, 800);
    widget->show();

    return app.exec();
}
