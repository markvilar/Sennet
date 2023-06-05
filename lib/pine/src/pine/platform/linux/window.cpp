#if defined(PINE_PLATFORM_LINUX)
#include "window.hpp"

#include "pine/pch.hpp"
#include "pine/core/event.hpp"
#include "pine/core/input.hpp"

#include "pine/platform/opengl/context.hpp"

namespace pine
{

static uint8_t s_glfw_window_count = 0;

static void GLFWErrorCallback(int error, const char* description)
{
    PINE_CORE_ERROR("GLFW error ({0}): {1}", error, description);
}

std::unique_ptr<Window> create_window(const WindowSpecs& specs)
{
    return std::make_unique<LinuxWindow>(specs);
}

LinuxWindow::LinuxWindow(const WindowSpecs& specs) : specification(specs) {}

LinuxWindow::~LinuxWindow() { shutdown(); }

void LinuxWindow::init()
{
    window_data.title = specification.title;
    window_data.width = specification.width;
    window_data.height = specification.height;

    if (s_glfw_window_count == 0)
    {
        [[maybe_unused]] int success = glfwInit();
        PINE_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    {
#if defined(PINE_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
        native_window = glfwCreateWindow(static_cast<int>(window_data.width),
            static_cast<int>(window_data.height),
            window_data.title.c_str(),
            nullptr,
            nullptr);
        ++s_glfw_window_count;
    }

    context = GraphicsContext::create(native_window);
    context->init();

    glfwSetWindowUserPointer(native_window, &window_data);
    set_vsync(true);

    glfwSetWindowSizeCallback(native_window,
        [](NativeWindow* window, int width, int height)
        {
            WindowData& data
                = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.width = static_cast<uint32_t>(width);
            data.height = static_cast<uint32_t>(height);

            WindowResized event;
            event.width = static_cast<uint32_t>(width); 
            event.height = static_cast<uint32_t>(height);
            data.event_callback(event);
        });

    glfwSetWindowIconifyCallback(native_window,
        [](NativeWindow* window, int iconified)
        {
            WindowData& data
                = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowIconified event;
            event.minimized = (iconified == 1);
            data.event_callback(event);
        });

    glfwSetWindowCloseCallback(native_window,
        [](NativeWindow* window)
        {
            WindowData& data
                = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowClosed event;
            data.event_callback(event);
        });

    glfwSetKeyCallback(native_window,
        [](NativeWindow* window,
            int key,
            [[maybe_unused]] int scancode,
            int action,
            [[maybe_unused]] int mods)
        {
            WindowData& data
                = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
            case GLFW_PRESS:
            {
                // TODO: Handle modifiers
                Pressed<Key> event;
                event.source.code = static_cast<KeyCode>(key);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                Released<Key> event;
                event.source.code = static_cast<KeyCode>(key);
                data.event_callback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                Pressed<Key> event;
                event.source.code = static_cast<KeyCode>(key);
                data.event_callback(event);
                break;
            }
            }
        });

    glfwSetCharCallback(native_window,
        [](NativeWindow* window, unsigned int key)
        {
            WindowData& data
                = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            
            Pressed<Key> event;
            event.source.code = static_cast<KeyCode>(key);
            data.event_callback(event);
        });

    glfwSetMouseButtonCallback(native_window,
        [](NativeWindow* window,
            int button,
            int action,
            [[maybe_unused]] int mods)
        {
            WindowData& data
                = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
            case GLFW_PRESS:
            {
                Pressed<MouseButton> event;
                event.source.code = static_cast<MouseCode>(button);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                Released<MouseButton> event;
                event.source.code = static_cast<MouseCode>(button);
                data.event_callback(event);
                break;
            }
            }
        });

    glfwSetScrollCallback(native_window,
        [](NativeWindow* window, const double offset_x, const double offset_y)
        {
            WindowData& data
                = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            
            Moved<MouseWheel> event;
            event.source.offset_x = static_cast<float>(offset_x);
            event.source.offset_y = static_cast<float>(offset_y);
            data.event_callback(event);
        });

    glfwSetCursorPosCallback(native_window,
        [](NativeWindow* window, const double pos_x, const double pos_y)
        {
            WindowData& data
                = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            Moved<Mouse> event;
            event.source.coordinate_x = static_cast<float>(pos_x);
            event.source.coordinate_y = static_cast<float>(pos_y);
            data.event_callback(event);
        });
}

void LinuxWindow::poll_events() { glfwPollEvents(); }

void LinuxWindow::swap_buffers() { context->swap_buffers(); }

std::pair<uint32_t, uint32_t> LinuxWindow::get_size() const
{
    return {window_data.width, window_data.height};
}

std::pair<float, float> LinuxWindow::get_position() const
{
    int x, y;
    glfwGetWindowPos(native_window, &x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}

void LinuxWindow::maximize() { glfwMaximizeWindow(native_window); }

void LinuxWindow::center_window()
{
    const auto& video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int x = video_mode->width / 2 - static_cast<int>(window_data.width) / 2;
    int y = video_mode->height / 2 - static_cast<int>(window_data.height) / 2;
    glfwSetWindowPos(native_window, x, y);
}

void LinuxWindow::set_event_callback(const EventCallbackFn& callback)
{
    window_data.event_callback = callback;
}

void LinuxWindow::set_vsync(const bool enabled)
{
    if (enabled)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }
    window_data.vsync = enabled;
}

bool LinuxWindow::is_vsync() const { return window_data.vsync; }

void LinuxWindow::set_resizable(const bool resizable) const
{
    glfwSetWindowAttrib(native_window,
        GLFW_RESIZABLE,
        resizable ? GLFW_TRUE : GLFW_FALSE);
}

const std::string& LinuxWindow::get_title() const { return window_data.title; }

void LinuxWindow::set_title(const std::string& title)
{
    window_data.title = title;
    glfwSetWindowTitle(native_window, window_data.title.c_str());
}

void LinuxWindow::shutdown()
{
    glfwDestroyWindow(native_window);
    --s_glfw_window_count;

    if (s_glfw_window_count == 0)
    {
        glfwTerminate();
    }
}

} // namespace pine

#endif
