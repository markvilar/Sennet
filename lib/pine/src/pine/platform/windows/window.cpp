#if defined(PINE_PLATFORM_WINDOWS)
#include "pine/platform/windows/window.hpp"

#include "pine/core/event.hpp"
#include "pine/core/input.hpp"
#include "pine/pch.hpp"

#include "pine/platform/opengl/context.hpp"

namespace pine {

static uint8_t s_glfw_window_count = 0;

static void GLFWErrorCallback(int error, const char* description) {
    PINE_CORE_ERROR("GLFW error ({0}): {1}", error, description);
}

std::unique_ptr<Window> create_window(const WindowSpecs& specs) {
    return std::make_unique<WindowsWindow>(specs);
}

WindowsWindow::WindowsWindow(const WindowSpecs& specs) : specification(specs) {}

WindowsWindow::~WindowsWindow() { shutdown(); }

void WindowsWindow::init() {
    window_data.title = specification.title;
    window_data.width = specification.width;
    window_data.height = specification.height;

    if (s_glfw_window_count == 0) {
        [[maybe_unused]] int success = glfwInit();
        PINE_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    {
#if defined(PINE_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
        window = glfwCreateWindow(static_cast<int>(window_data.width),
            static_cast<int>(window_data.height),
            window_data.title.c_str(),
            nullptr,
            nullptr);
        ++s_glfw_window_count;
    }

    context = GraphicsContext::create(window);
    context->init();

    glfwSetWindowUserPointer(window, &window_data);
    set_vsync(true);

    glfwSetWindowSizeCallback(window,
        [](GLFWwindow* reference_window, int width, int height) {
            WindowData& data = *static_cast<WindowData*>(
                glfwGetWindowUserPointer(reference_window));
            data.width = static_cast<uint32_t>(width);
            data.height = static_cast<uint32_t>(height);
            WindowResizeEvent event(static_cast<uint32_t>(width),
                static_cast<uint32_t>(height));
            data.event_callback(event);
        });

    glfwSetWindowIconifyCallback(window,
        [](GLFWwindow* reference_window, int iconified) {
            WindowData& data = *static_cast<WindowData*>(
                glfwGetWindowUserPointer(reference_window));
            WindowIconifyEvent event(iconified == 1);
            data.event_callback(event);
        });

    glfwSetWindowCloseCallback(window, [](GLFWwindow* reference_window) {
        WindowData& data = *static_cast<WindowData*>(
            glfwGetWindowUserPointer(reference_window));
        WindowCloseEvent event;
        data.event_callback(event);
    });

    glfwSetKeyCallback(window,
        [](GLFWwindow* reference_window,
            int key,
            [[maybe_unused]] int scancode,
            [[maybe_unused]] int action,
            [[maybe_unused]] int mods) {
            WindowData& data = *static_cast<WindowData*>(
                glfwGetWindowUserPointer(reference_window));
            switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(static_cast<KeyCode>(key), 0);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(static_cast<KeyCode>(key));
                data.event_callback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(static_cast<KeyCode>(key), 1);
                data.event_callback(event);
                break;
            }
            }
        });

    glfwSetCharCallback(window,
        [](GLFWwindow* reference_window, unsigned int key) {
            WindowData& data = *static_cast<WindowData*>(
                glfwGetWindowUserPointer(reference_window));
            KeyTypedEvent event(static_cast<KeyCode>(key));
            data.event_callback(event);
        });

    glfwSetMouseButtonCallback(window,
        [](GLFWwindow* reference_window,
            int button,
            [[maybe_unused]] int action,
            [[maybe_unused]] int mods) {
            WindowData& data = *static_cast<WindowData*>(
                glfwGetWindowUserPointer(reference_window));
            switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
                data.event_callback(event);
                break;
            }
            }
        });

    glfwSetScrollCallback(window,
        [](GLFWwindow* reference_window, double offset_x, double offset_y) {
            WindowData& data = *static_cast<WindowData*>(
                glfwGetWindowUserPointer(reference_window));
            MouseScrolledEvent event(static_cast<float>(offset_x),
                static_cast<float>(offset_y));
            data.event_callback(event);
        });

    glfwSetCursorPosCallback(reference_window,
        [](GLFWwindow* reference_window, double pos_x, double pos_y) {
            WindowData& data = *static_cast<WindowData*>(
                glfwGetWindowUserPointer(reference_window));
            MouseMovedEvent event(static_cast<float>(pos_x),
                static_cast<float>(pos_y));
            data.event_callback(event);
        });
}

void WindowsWindow::poll_events() { glfwPollEvents(); }

void WindowsWindow::swap_buffers() { context->swap_buffers(); }

std::pair<uint32_t, uint32_t> WindowsWindow::get_size() const {
    return {window_data.width, window_data.height};
}

std::pair<float, float> WindowsWindow::get_position() const {
    int x, y;
    glfwGetWindowPos(window, &x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}

void WindowsWindow::maximize() { glfwMaximizeWindow(window); }

void WindowsWindow::center_window() {
    const GLFWvidmode* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int x = (video_mode->width / 2) - static_cast<int>((window_data.width / 2));
    int y =
        (video_mode->height / 2) - static_cast<int>((window_data.height / 2));
    glfwSetWindowPos(window, x, y);
}

void WindowsWindow::set_event_callback(const EventCallbackFn& callback) {
    window_data.event_callback = callback;
}

void WindowsWindow::set_vsync(const bool enabled) {
    if (enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
    window_data.vsync = enabled;
}

bool WindowsWindow::is_vsync() const { return window_data.vsync; }

void WindowsWindow::set_resizable(const bool resizable) const {
    glfwSetWindowAttrib(window,
        GLFW_RESIZABLE,
        resizable ? GLFW_TRUE : GLFW_FALSE);
}

const std::string& WindowsWindow::get_title() const {
    return window_data.title;
}

void WindowsWindow::set_title(const std::string& title) {
    window_data.title = title;
    glfwSetWindowTitle(window, window_data.title.c_str());
}

void WindowsWindow::shutdown() {
    glfwDestroyWindow(window);
    --s_glfw_window_count;

    if (s_glfw_window_count == 0) {
        glfwTerminate();
    }
}

} // namespace pine

#endif
