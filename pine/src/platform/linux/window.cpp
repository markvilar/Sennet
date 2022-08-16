#if defined(PINE_PLATFORM_LINUX)
#include "pine/platform/linux/window.hpp"

#include "pine/core/input.hpp"
#include "pine/events/application_event.hpp"
#include "pine/events/key_event.hpp"
#include "pine/events/mouse_event.hpp"
#include "pine/pch.hpp"
#include "pine/platform/opengl/context.hpp"

namespace pine
{

static uint8_t s_glfw_window_count = 0;

static void GLFWErrorCallback(int error, const char* description)
{
    PINE_CORE_ERROR("GLFW error ({0}): {1}", error, description);
}

std::unique_ptr<Window> Window::create(const WindowSpecs& specs)
{
    return std::make_unique<LinuxWindow>(specs);
}

LinuxWindow::LinuxWindow(const WindowSpecs& specs) : m_specification(specs) {}

LinuxWindow::~LinuxWindow() { shutdown(); }

void LinuxWindow::init()
{
    m_data.title = m_specification.title;
    m_data.width = m_specification.width;
    m_data.height = m_specification.height;

    if (s_glfw_window_count == 0)
    {
        int success = glfwInit();
        PINE_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    {
#if defined(PINE_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
        m_window = glfwCreateWindow(static_cast<int>(m_data.width),
            static_cast<int>(m_data.height),
            m_data.title.c_str(),
            nullptr,
            nullptr);
        ++s_glfw_window_count;
    }

    m_context = GraphicsContext::create(m_window);
    m_context->init();

    glfwSetWindowUserPointer(m_window, &m_data);
    set_vsync(true);

    glfwSetWindowSizeCallback(m_window,
        [](GLFWwindow* window, int width, int height)
        {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.width = static_cast<uint32_t>(width);
            data.height = static_cast<uint32_t>(height);
            WindowResizeEvent event(static_cast<uint32_t>(width),
                static_cast<uint32_t>(height));
            data.event_callback(event);
        });

    glfwSetWindowIconifyCallback(m_window,
        [](GLFWwindow* window, int iconified)
        {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowIconifyEvent event(iconified == 1);
            data.event_callback(event);
        });

    glfwSetWindowCloseCallback(m_window,
        [](GLFWwindow* window)
        {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data.event_callback(event);
        });

    glfwSetKeyCallback(m_window,
        [](GLFWwindow* window,
            int key,
            [[maybe_unused]] int scancode,
            int action,
            [[maybe_unused]] int mods)
        {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(static_cast<KeyCode>(key), 0);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(static_cast<KeyCode>(key));
                data.event_callback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(static_cast<KeyCode>(key), 1);
                data.event_callback(event);
                break;
            }
            }
        });

    glfwSetCharCallback(m_window,
        [](GLFWwindow* window, unsigned int key)
        {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(static_cast<KeyCode>(key));
            data.event_callback(event);
        });

    glfwSetMouseButtonCallback(m_window,
        [](GLFWwindow* window,
            int button,
            int action,
            [[maybe_unused]] int mods)
        {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
                data.event_callback(event);
                break;
            }
            }
        });

    glfwSetScrollCallback(m_window,
        [](GLFWwindow* window, const double offset_x, const double offset_y)
        {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event(static_cast<float>(offset_x),
                static_cast<float>(offset_y));
            data.event_callback(event);
        });

    glfwSetCursorPosCallback(m_window,
        [](GLFWwindow* window, const double pos_x, const double pos_y)
        {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(static_cast<float>(pos_x),
                static_cast<float>(pos_y));
            data.event_callback(event);
        });
}

void LinuxWindow::poll_events() { glfwPollEvents(); }

void LinuxWindow::swap_buffers() { m_context->swap_buffers(); }

std::pair<uint32_t, uint32_t> LinuxWindow::get_size() const
{
    return {m_data.width, m_data.height};
}

std::pair<float, float> LinuxWindow::get_position() const
{
    int x, y;
    glfwGetWindowPos(m_window, &x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}

void LinuxWindow::maximize() { glfwMaximizeWindow(m_window); }

void LinuxWindow::center_window()
{
    const GLFWvidmode* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int x = video_mode->width / 2 - static_cast<int>(m_data.width) / 2;
    int y = video_mode->height / 2 - static_cast<int>(m_data.height) / 2;
    glfwSetWindowPos(m_window, x, y);
}

void LinuxWindow::set_event_callback(const EventCallbackFn& callback)
{
    m_data.event_callback = callback;
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
    m_data.vsync = enabled;
}

bool LinuxWindow::is_vsync() const { return m_data.vsync; }

void LinuxWindow::set_resizable(const bool resizable) const
{
    glfwSetWindowAttrib(m_window,
        GLFW_RESIZABLE,
        resizable ? GLFW_TRUE : GLFW_FALSE);
}

const std::string& LinuxWindow::get_title() const { return m_data.title; }

void LinuxWindow::set_title(const std::string& title)
{
    m_data.title = title;
    glfwSetWindowTitle(m_window, m_data.title.c_str());
}

void LinuxWindow::shutdown()
{
    glfwDestroyWindow(m_window);
    --s_glfw_window_count;

    if (s_glfw_window_count == 0)
    {
        glfwTerminate();
    }
}

} // namespace pine

#endif
