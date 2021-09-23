#include "Platform/Windows/WindowsWindow.hpp"
#include "Sennet/Pch.hpp"

#include "Sennet/Core/Input.hpp"

#include "Sennet/Events/ApplicationEvent.hpp"
#include "Sennet/Events/KeyEvent.hpp"
#include "Sennet/Events/MouseEvent.hpp"

#include "Platform/OpenGL/OpenGLContext.hpp"

namespace Sennet
{

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
    SENNET_CORE_ERROR("GLFW error ({0}): {1}", error, description);
}

WindowsWindow::WindowsWindow(const WindowProperties& props) { Init(props); }

WindowsWindow::~WindowsWindow() { Shutdown(); }

void WindowsWindow::OnUpdate()
{
    glfwPollEvents();
    m_Context->SwapBuffers();
}

void WindowsWindow::SetVSync(bool enabled)
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

void WindowsWindow::Init(const WindowProperties& props)
{
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    SENNET_CORE_INFO("Creating window {0}, ({1}, {2})",
        props.Title,
        props.Width,
        props.Height);

    if (s_GLFWWindowCount == 0)
    {
        SENNET_CORE_INFO("Initializing GLFW.");
        int success = glfwInit();
        SENNET_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    {
#if defined(SENNET_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
        m_Window = glfwCreateWindow((int)props.Width,
            (int)props.Height,
            m_Data.Title.c_str(),
            nullptr,
            nullptr);
        ++s_GLFWWindowCount;
    }

    m_Context = GraphicsContext::Create(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    glfwSetWindowSizeCallback(
        m_Window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods) {
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

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        KeyTypedEvent event(static_cast<KeyCode>(keycode));
        data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(
        m_Window, [](GLFWwindow* window, int button, int action, int mods) {
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

    glfwSetScrollCallback(
        m_Window, [](GLFWwindow* window, double offsetX, double offsetY) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)offsetX, (float)offsetY);
            data.EventCallback(event);
        });

    glfwSetCursorPosCallback(
        m_Window, [](GLFWwindow* window, double posX, double posY) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)posX, (float)posY);
            data.EventCallback(event);
        });
}

void WindowsWindow::Shutdown()
{
    SENNET_CORE_INFO("Deleting window {0}, ({1}, {2})",
        m_Data.Title,
        m_Data.Width,
        m_Data.Height);

    glfwDestroyWindow(m_Window);
    --s_GLFWWindowCount;

    if (s_GLFWWindowCount == 0)
    {
        SENNET_CORE_INFO("Terminating GLFW");
        glfwTerminate();
    }
}

} // namespace Sennet
