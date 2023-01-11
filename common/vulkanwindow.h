#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H

#include <QWidget>
#include <QVulkanWindow>
#include "backends/imgui_impl_vulkan.h"

class QVulkanDeviceFunctions;
class VulkanWindow;

class ImGuiVulkanWidget: public QWidget
{
    Q_OBJECT
public:
    ImGuiVulkanWidget(VulkanWindow* w, QWidget* parent = NULL);
    void set_clear_color(ImVec4 color);
    ImVec4 clear_color() const;
    VulkanWindow* window() const;

signals:
    void sig_window_event(QEvent* e);

protected:
    virtual void init_window(){};
    virtual void paint();
    virtual void release_window(){};
    virtual void vulkan_window_ready(){};

private slots:
    void slot_readypaint();
    void slot_window_init();
    void slot_window_release();
    void slot_vulkan_window_ready();

private:
    VulkanWindow *m_window;
};

class QVulkanRenderer : public QVulkanWindowRenderer
{
public:
    QVulkanRenderer(QVulkanWindow* w);

    void initResources() override;
    void startNextFrame() override;
    void releaseResources() override;

protected:
    void on_mouse_pressed_change(QMouseEvent* event);
    void on_wheel(QWheelEvent* event);
    void on_key_press_release(QKeyEvent* event);

private:
    void init();
    void update();

    void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
    void FramePresent(ImGui_ImplVulkanH_Window* wd);

    void imgui_qvulkan_new_frame();
    void set_cursor_pos(const ImGuiIO& io);
    void update_cursor_shape(const ImGuiIO& io);

protected:
    QVulkanWindow *m_window;
    QVulkanDeviceFunctions* m_dev_func;
    QVulkanFunctions* m_func;
    ImGui_ImplVulkanH_Window* wd_;
    ImGuiContext* ctx;
    // stage
    VkDescriptorPool vk_descript_pool_ = VK_NULL_HANDLE;

    double       gtime = 0.0f;
    bool         g_mouse_pressed[3] = { false, false, false };
    float        g_mouse_wheel;
    float        g_mouse_wheel_h;

private:
    bool inited_ = false;

private:
    friend class VulkanWindow;
};

class VulkanWindow : public QVulkanWindow
{
    Q_OBJECT
public:
    QVulkanWindowRenderer *createRenderer() override;
    bool event(QEvent *) override;
    void set_clear_color(ImVec4 color);
    ImVec4 clear_color() const;
    VkDescriptorPool descript_pool() const;
signals:
      void sig_readypaint();
      void sig_window_init();
      void sig_window_release();
      void sig_window_event(QEvent *);
      // qvulkanwindow ready like cmdbuf
      void sig_window_init_ready();

private:
      QVulkanWindowRenderer* render_ = NULL;
      ImVec4 clear_color_;

private:
      friend class QVulkanRenderer;

};

#endif // VULKANWINDOW_H
