#include "vulkanwindow.h"
#include <QLayout>
#include <QObject>
#include <QVulkanDeviceFunctions>
#include <QDateTime>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include "imgui.h"

const QHash<int, ImGuiKey> keyMap = {
    { Qt::Key_Tab, ImGuiKey_Tab },
    { Qt::Key_Left, ImGuiKey_LeftArrow },
    { Qt::Key_Right, ImGuiKey_RightArrow },
    { Qt::Key_Up, ImGuiKey_UpArrow },
    { Qt::Key_Down, ImGuiKey_DownArrow },
    { Qt::Key_PageUp, ImGuiKey_PageUp },
    { Qt::Key_PageDown, ImGuiKey_PageDown },
    { Qt::Key_Home, ImGuiKey_Home },
    { Qt::Key_End, ImGuiKey_End },
    { Qt::Key_Insert, ImGuiKey_Insert },
    { Qt::Key_Delete, ImGuiKey_Delete },
    { Qt::Key_Backspace, ImGuiKey_Backspace },
    { Qt::Key_Space, ImGuiKey_Space },
    { Qt::Key_Enter, ImGuiKey_Enter },
    { Qt::Key_Return, ImGuiKey_Enter },
    { Qt::Key_Escape, ImGuiKey_Escape },
    { Qt::Key_A, ImGuiKey_A },
    { Qt::Key_C, ImGuiKey_C },
    { Qt::Key_V, ImGuiKey_V },
    { Qt::Key_X, ImGuiKey_X },
    { Qt::Key_Y, ImGuiKey_Y },
    { Qt::Key_Z, ImGuiKey_Z },
    { Qt::MiddleButton, ImGuiMouseButton_Middle }
};

#ifndef QT_NO_CURSOR
const QHash<ImGuiMouseCursor, Qt::CursorShape> cursorMap = {
    { ImGuiMouseCursor_Arrow,      Qt::CursorShape::ArrowCursor },
    { ImGuiMouseCursor_TextInput,  Qt::CursorShape::IBeamCursor },
    { ImGuiMouseCursor_ResizeAll,  Qt::CursorShape::SizeAllCursor },
    { ImGuiMouseCursor_ResizeNS,   Qt::CursorShape::SizeVerCursor },
    { ImGuiMouseCursor_ResizeEW,   Qt::CursorShape::SizeHorCursor },
    { ImGuiMouseCursor_ResizeNESW, Qt::CursorShape::SizeBDiagCursor },
    { ImGuiMouseCursor_ResizeNWSE, Qt::CursorShape::SizeFDiagCursor },
    { ImGuiMouseCursor_Hand,       Qt::CursorShape::PointingHandCursor },
    { ImGuiMouseCursor_NotAllowed, Qt::CursorShape::ForbiddenCursor },
};
#endif

static void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

QVulkanRenderer::QVulkanRenderer(QVulkanWindow *w)
    :m_window(w)
    , wd_(new ImGui_ImplVulkanH_Window)
{

}

void QVulkanRenderer::initResources()
{
    auto device = m_window->device();
    m_dev_func = m_window->vulkanInstance()->deviceFunctions(device);
    m_func = m_window->vulkanInstance()->functions();

    ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);
    ImGuiIO &io = ImGui::GetIO();
    #ifndef QT_NO_CURSOR
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
    #endif
    io.BackendPlatformName = "qvulkan imgui";

    // Setup keyboard mapping
    for (ImGuiKey key : keyMap.values()) {
        io.KeyMap[key] = key;
    }

    emit dynamic_cast<VulkanWindow*>(m_window)->sig_window_init();
}

void QVulkanRenderer::startNextFrame()
{
    if(!inited_){
        init();
        inited_ = true;
    } else {
        update();
    }
    m_window->frameReady();
    m_window->requestUpdate();
}

void QVulkanRenderer::releaseResources()
{
    auto device = m_window->device();

    auto err = m_dev_func->vkDeviceWaitIdle(device);
    check_vk_result(err);
    ImGui_ImplVulkan_Shutdown();
    ImGui::DestroyContext();

    ImGui_ImplVulkanH_DestroyQWindow(device, wd_, NULL);

    emit dynamic_cast<VulkanWindow*>(m_window)->sig_window_release();
}

void QVulkanRenderer::on_mouse_pressed_change(QMouseEvent *event)
{
    g_mouse_pressed[0] = event->buttons() & Qt::LeftButton;
    g_mouse_pressed[1] = event->buttons() & Qt::RightButton;
    g_mouse_pressed[2] = event->buttons() & Qt::MiddleButton;
}

void QVulkanRenderer::on_wheel(QWheelEvent *event)
{
    // Select current context
    ImGui::SetCurrentContext(ctx);

    // Handle horizontal component
    if(event->pixelDelta().x() != 0)
    {
        g_mouse_wheel_h += event->pixelDelta().x() / (ImGui::GetTextLineHeight());
    } else {
        // Magic number of 120 comes from Qt doc on QWheelEvent::pixelDelta()
        g_mouse_wheel_h += event->angleDelta().x() / 120;
    }

    // Handle vertical component
    if(event->pixelDelta().y() != 0)
    {
        // 5 lines per unit
        g_mouse_wheel += event->pixelDelta().y() / (5.0 * ImGui::GetTextLineHeight());
    } else {
        // Magic number of 120 comes from Qt doc on QWheelEvent::pixelDelta()
        g_mouse_wheel += event->angleDelta().y() / 120;
    }
}

void QVulkanRenderer::on_key_press_release(QKeyEvent *event)
{
    // Select current context
    ImGui::SetCurrentContext(ctx);

    ImGuiIO& io = ImGui::GetIO();

    const bool key_pressed = (event->type() == QEvent::KeyPress);

    // Translate `Qt::Key` into `ImGuiKey`, and apply 'pressed' state for that key
    const auto key_it = keyMap.constFind( event->key() );
    if (key_it != keyMap.constEnd()) { // Qt's key found in keyMap
        const int imgui_key = *(key_it);
        io.KeysDown[imgui_key] = key_pressed;
    }

    if (key_pressed) {
        const QString text = event->text();
        if (text.size() == 1) {
            io.AddInputCharacter( text.at(0).unicode() );
        }
    }

#ifdef Q_OS_MAC
    io.KeyCtrl  = event->modifiers() & Qt::MetaModifier;
    io.KeyShift = event->modifiers() & Qt::ShiftModifier;
    io.KeyAlt   = event->modifiers() & Qt::AltModifier;
    io.KeySuper = event->modifiers() & Qt::ControlModifier; // Comamnd key
#else
    io.KeyCtrl  = event->modifiers() & Qt::ControlModifier;
    io.KeyShift = event->modifiers() & Qt::ShiftModifier;
    io.KeyAlt   = event->modifiers() & Qt::AltModifier;
    io.KeySuper = event->modifiers() & Qt::MetaModifier;
#endif
}

void QVulkanRenderer::init()
{
    auto surface = QVulkanInstance::surfaceForWindow(m_window);
    auto pdevice = m_window->physicalDevice();
    auto instance = m_window->vulkanInstance()->vkInstance();
    auto device = m_window->device();
    auto queue = m_window->graphicsQueue();
    auto renderpass = m_window->defaultRenderPass();
//    auto cmdpool = m_window->graphicsCommandPool();
    auto cmdbuf = m_window->currentCommandBuffer();
//    VkDescriptorPool dpool = VK_NULL_HANDLE;

    auto qfamily = 0;
    {
        uint32_t count;
        m_func->vkGetPhysicalDeviceQueueFamilyProperties(pdevice, &count, NULL);
        VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
        m_func->vkGetPhysicalDeviceQueueFamilyProperties(pdevice, &count, queues);
        for (uint32_t i = 0; i < count; i++){
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                qfamily = i;
                break;
            }
        }
        free(queues);
    }

    wd_->Surface = surface;
    wd_->RenderPass = renderpass;
    const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

    wd_->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(pdevice, surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

#ifdef IMGUI_UNLIMITED_FRAME_RATE
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
    wd_->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(pdevice, surface, &present_modes[0], IM_ARRAYSIZE(present_modes));

    int min_image_count = 2;

    {
        VkResult err;
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        err = m_dev_func->vkCreateDescriptorPool(device, &pool_info, NULL, &vk_descript_pool_);
        check_vk_result(err);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    // platform init

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = instance;
    init_info.PhysicalDevice = pdevice;
    init_info.Device = device;
    init_info.QueueFamily = qfamily;
    init_info.Queue = queue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = vk_descript_pool_;
    init_info.Subpass = 0;
    init_info.MinImageCount = min_image_count;
    init_info.ImageCount = min_image_count;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = NULL;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, wd_->RenderPass);

    io.Fonts->AddFontFromFileTTF("D:/github/QtImGUIVk/3rdparty/font/simhei.ttf", 13, NULL, io.Fonts->GetGlyphRangesDefault());
    {
        ImGui_ImplVulkan_CreateFontsTexture(cmdbuf);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
//    io.Fonts->AddFontFromFileTTF("D:/github/QtImGUIVk/3rdparty/font/simhei.ttf", 13, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
//    io.Fonts->Build();

    // after imgui init
    emit dynamic_cast<VulkanWindow*>(m_window)->sig_window_init_ready();
}

void QVulkanRenderer::update()
{
    auto window = dynamic_cast<VulkanWindow*>(m_window);
    if(!window) return;
    ImVec4 clear_color = window->clear_color_;

    imgui_qvulkan_new_frame();
    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui::NewFrame();
    // notice other window or widget to draw imgui
    emit window->sig_readypaint();
    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
    if (!is_minimized)
    {
        wd_->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
        wd_->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
        wd_->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
        wd_->ClearValue.color.float32[3] = clear_color.w;
        FrameRender(wd_, draw_data);
//        FramePresent(wd_);
    }
}

void QVulkanRenderer::FrameRender(ImGui_ImplVulkanH_Window *wd, ImDrawData *draw_data)
{
    auto cmdbuf = m_window->currentCommandBuffer();
    auto fb = m_window->currentFramebuffer();
    auto sz = m_window->swapChainImageSize();
    auto renderpass = m_window->defaultRenderPass();

    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = renderpass;
        info.framebuffer = fb;
        info.renderArea.extent.width = sz.width();
        info.renderArea.extent.height = sz.height();
        info.clearValueCount = m_window->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
        info.pClearValues = &wd->ClearValue;
        m_dev_func->vkCmdBeginRenderPass(cmdbuf, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(draw_data, cmdbuf);
    // Submit command buffer
    m_dev_func->vkCmdEndRenderPass(cmdbuf);
}

void QVulkanRenderer::FramePresent(ImGui_ImplVulkanH_Window *wd)
{
    return;
}

void QVulkanRenderer::imgui_qvulkan_new_frame()
{
    auto sz = m_window->swapChainImageSize();
    ImGuiIO& io = ImGui::GetIO();
    int w, h;
    int display_w, display_h;
    w = m_window->width();
    h = m_window->height();
    display_w = sz.width();
    display_h = sz.height();
    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / (float)w, (float)display_h / (float)h);
    auto current_time = QDateTime::currentMSecsSinceEpoch();
    io.DeltaTime = gtime > 0.0 ? (float)(current_time - gtime) : (float)(1.0f / 60.0f);

    set_cursor_pos(io);

    if (m_window->isActive())
    {
        const QPoint pos = m_window->mapFromGlobal(QCursor::pos());
        io.MousePos = ImVec2(pos.x(), pos.y());   // Mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)
    }
    else
    {
        io.MousePos = ImVec2(-1,-1);
    }

    for (int i = 0; i < 3; i++)
    {
        io.MouseDown[i] = g_mouse_pressed[i];
    }

    io.MouseWheelH = g_mouse_wheel_h;
    io.MouseWheel = g_mouse_wheel;
    g_mouse_wheel_h = 0;
    g_mouse_wheel = 0;

    update_cursor_shape(io);
}

void QVulkanRenderer::set_cursor_pos(const ImGuiIO &io)
{
    // NOTE: This code will be executed, only if the following flags have been set:
    // - backend flag: `ImGuiBackendFlags_HasSetMousePos`      - enabled
    // - config  flag: `ImGuiConfigFlags_NavEnableSetMousePos` - enabled

#ifndef QT_NO_CURSOR
    if(io.WantSetMousePos) {
        const QPoint global_pos = m_window->mapToGlobal({(int)io.MousePos.x, (int)io.MousePos.y});
        auto cursor = m_window->cursor();
        cursor.setPos(global_pos);
        m_window->setCursor(cursor);
    }
#else
    Q_UNUSED(io);
#endif
}

void QVulkanRenderer::update_cursor_shape(const ImGuiIO &io)
{
    // NOTE: This code will be executed, only if the following flags have been set:
    // - backend flag: `ImGuiBackendFlags_HasMouseCursors`    - enabled
    // - config  flag: `ImGuiConfigFlags_NoMouseCursorChange` - disabled

#ifndef QT_NO_CURSOR
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    const ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || (imgui_cursor == ImGuiMouseCursor_None))
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        m_window->setCursor(Qt::CursorShape::BlankCursor);
    }
    else
    {
        // Show OS mouse cursor

        // Translate `ImGuiMouseCursor` into `Qt::CursorShape` and show it, if we can
        const auto cursor_it = cursorMap.constFind( imgui_cursor );
        if(cursor_it != cursorMap.constEnd()) // `Qt::CursorShape` found for `ImGuiMouseCursor`
        {
            const Qt::CursorShape qt_cursor_shape = *(cursor_it);
            m_window->setCursor(qt_cursor_shape);
        }
        else // shape NOT found - use default
        {
            m_window->setCursor(Qt::CursorShape::ArrowCursor);
        }
    }
#else
    Q_UNUSED(io);
#endif
}

QVulkanWindowRenderer *VulkanWindow::createRenderer()
{
    if(render_)
    {
        delete render_;
        render_ = nullptr;
    }
    render_ = new QVulkanRenderer(this);
    return render_;
}

bool VulkanWindow::event(QEvent *e)
{
    auto res = QVulkanWindow::event(e);
    emit sig_window_event(e);

    auto rd = dynamic_cast<QVulkanRenderer*>(render_);
    switch (e->type()) {
    default:
        break;
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
        if(rd) rd->on_mouse_pressed_change(static_cast<QMouseEvent*>(e)); break;
    case QEvent::Wheel:
        if(rd) rd->on_wheel(static_cast<QWheelEvent*>(e)); break;
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
        if(rd) rd->on_key_press_release(static_cast<QKeyEvent*>(e)); break;
    }

    return res;
}

void VulkanWindow::set_clear_color(ImVec4 color)
{
    clear_color_ = color;
}

ImVec4 VulkanWindow::clear_color() const
{
    return clear_color_;
}

VkDescriptorPool VulkanWindow::descript_pool() const
{
    auto rd = dynamic_cast<QVulkanRenderer*>(render_);
    if(rd) return rd->vk_descript_pool_;
    return VK_NULL_HANDLE;
}

ImGuiVulkanWidget::ImGuiVulkanWidget(VulkanWindow *w, QWidget *parent)
    : QWidget(parent)
    , m_window(w)
{
    m_window->set_clear_color(ImVec4(0.45f, 0.55f, 0.60f, 1.00f));
    auto wapper = QWidget::createWindowContainer(w, parent);
    connect(m_window, &VulkanWindow::sig_readypaint, this, &ImGuiVulkanWidget::slot_readypaint);
    connect(m_window, &VulkanWindow::sig_window_init, this, &ImGuiVulkanWidget::slot_window_init);
    connect(m_window, &VulkanWindow::sig_window_release, this, &ImGuiVulkanWidget::slot_window_release);
    connect(m_window, &VulkanWindow::sig_window_event, this, &ImGuiVulkanWidget::sig_window_event);
    connect(m_window, &VulkanWindow::sig_window_init_ready, this, &ImGuiVulkanWidget::slot_vulkan_window_ready);
    auto layout = new QHBoxLayout;
    layout->addWidget(wapper);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
}

void ImGuiVulkanWidget::set_clear_color(ImVec4 color)
{
    m_window->set_clear_color(color);
}

ImVec4 ImGuiVulkanWidget::clear_color() const
{
    return m_window->clear_color();
}

VulkanWindow *ImGuiVulkanWidget::window() const
{
    return m_window;
}

void ImGuiVulkanWidget::paint()
{
#ifdef DRAW_IMGUI_DEMO
    auto clear_color_ = clear_color();
    static bool show_demo_window = true;
    static bool show_another_window = false;
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color_); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
#endif
}

void ImGuiVulkanWidget::slot_readypaint()
{
    paint();
}

void ImGuiVulkanWidget::slot_window_init()
{
    init_window();
}

void ImGuiVulkanWidget::slot_window_release()
{
    release_window();
}

void ImGuiVulkanWidget::slot_vulkan_window_ready()
{
    vulkan_window_ready();
}
