#if defined(PINE_PLATFORM_WINDOWS)
#include "pine/platform/windows/window.hpp"

#include "pine/core/input.hpp"
#include "pine/events/application_event.hpp"
#include "pine/events/key_event.hpp"
#include "pine/events/mouse_event.hpp"
#include "pine/pch.hpp"
#include "pine/platform/opengl/context.hpp"

namespace pine
{

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
    PINE_CORE_ERROR("GLFW error ({0}): {1}", error, description);
}

std::unique_ptr<Window> Window::Create(const Window::Specification& specs)
{
    return std::make_unique<WindowsWindow>(specs);
}

WindowsWindow::WindowsWindow(const Window::Specification& specs)
    : m_Specification(specs)
{
}

WindowsWindow::~WindowsWindow() { Shutdown(); }

void WindowsWindow::Init()
{
    m_Data.Title = m_Specification.Title;
    m_Data.Width = m_Specification.Width;
    m_Data.Height = m_Specification.Height;

    if (s_GLFWWindowCount == 0)
    {
        int success = glfwInit();
        PINE_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    {
#if defined(PINE_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
        m_Window = glfwCreateWindow((int)m_Data.Width,
            (int)m_Data.Height,
            m_Data.Title.c_str(),
            nullptr,
            nullptr);
        ++s_GLFWWindowCount;
    }

    m_Context =
        std::unique_ptr<GraphicsContext>(GraphicsContext::Create(m_Window));
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    glfwSetWindowSizeCallback(m_Window,
        [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;
            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

    glfwSetWindowIconifyCallback(m_Window,
        [](GLFWwindow* window, int iconified)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowIconifyEvent event(iconified == 1);
            data.EventCallback(event);
        });

    glfwSetWindowCloseCallback(m_Window,
        [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

    glfwSetKeyCallback(m_Window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(static_cast<KeyCode>(key), 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(static_cast<KeyCode>(key));
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(static_cast<KeyCode>(key), 1);
                data.EventCallback(event);
                break;
            }
            }
        });

    glfwSetCharCallback(m_Window,
        [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(static_cast<KeyCode>(keycode));
            data.EventCallback(event);
        });

    glfwSetMouseButtonCallback(m_Window,
        [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
                data.EventCallback(event);
                break;
            }
            }
        });

    glfwSetScrollCallback(m_Window,
        [](GLFWwindow* window, double offsetX, double offsetY)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)offsetX, (float)offsetY);
            data.EventCallback(event);
        });

    glfwSetCursorPosCallback(m_Window,
        [](GLFWwindow* window, double posX, double posY)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)posX, (float)posY);
            data.EventCallback(event);
        });
}

void WindowsWindow::PollEvents() { glfwPollEvents(); }

void WindowsWindow::SwapBuffers() { m_Context->SwapBuffers(); }

std::pair<uint32_t, uint32_t> WindowsWindow::GetSize() const
{
    return {m_Data.Width, m_Data.Height};
}

std::pair<float, float> WindowsWindow::GetWindowPos() const
{
    int x, y;
    glfwGetWindowPos(m_Window, &x, &y);
    return {(float)x, (float)y};
}

void WindowsWindow::Maximize() { glfwMaximizeWindow(m_Window); }

void WindowsWindow::CenterWindow()
{
    const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int x = (videoMode->width / 2) - (m_Data.Width / 2);
    int y = (videoMode->height / 2) - (m_Data.Height / 2);
    glfwSetWindowPos(m_Window, x, y);
}

void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
{
    m_Data.EventCallback = callback;
}

void WindowsWindow::SetVSync(const bool enabled)
{
    if (enabled)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }
    m_Data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const { return m_Data.VSync; }

void WindowsWindow::SetResizable(const bool resizable) const
{
    glfwSetWindowAttrib(m_Window,
        GLFW_RESIZABLE,
        resizable ? GLFW_TRUE : GLFW_FALSE);
}

const std::string& WindowsWindow::GetTitle() const { return m_Data.Title; }

void WindowsWindow::SetTitle(const std::string& title)
{
    m_Data.Title = title;
    glfwSetWindowTitle(m_Window, m_Data.Title.c_str());
}

void WindowsWindow::Shutdown()
{
    glfwDestroyWindow(m_Window);
    --s_GLFWWindowCount;

    if (s_GLFWWindowCount == 0)
    {
        glfwTerminate();
    }
}

} // namespace pine

#endif
