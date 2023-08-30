#if defined(PINE_PLATFORM_LINUX)
#include "pine/platform/linux/window.hpp"

#include <GLFW/glfw3.h>

#include "pine/core/event.hpp"
#include "pine/core/input.hpp"
#include "pine/core/log.hpp"
#include "pine/pch.hpp"

namespace pine {

static uint8_t window_count = 0;

static void error_callback(int error, const char* description) {
    PINE_CORE_ERROR("GLFW error ({0}): {1}", error, description);
}

std::unique_ptr<Window> create_window(const WindowSpecs& specs) {
    return std::make_unique<LinuxWindow>(specs);
}

LinuxWindow::LinuxWindow(const WindowSpecs& specs) : specification(specs) {}

LinuxWindow::~LinuxWindow() { shutdown(); }

void LinuxWindow::init() {
    window_data.title = specification.title;
    window_data.width = specification.width;
    window_data.height = specification.height;

    // TODO: Move to window context
    if (window_count == 0) {
        [[maybe_unused]] int success = glfwInit();
        PINE_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(error_callback);
    }

    {
#if defined(PINE_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
        native_window = static_cast<void*>(
            glfwCreateWindow(static_cast<int>(window_data.width),
                static_cast<int>(window_data.height),
                window_data.title.c_str(),
                nullptr,
                nullptr));
        ++window_count;
    }

    set_callbacks();
}

void LinuxWindow::poll_events() { glfwPollEvents(); }

void LinuxWindow::swap_buffers() { glfwSwapBuffers(static_cast<GLFWwindow*>(native_window)); }

std::pair<uint32_t, uint32_t> LinuxWindow::get_size() const {
    return {window_data.width, window_data.height};
}

std::pair<float, float> LinuxWindow::get_position() const {
    int x, y;
    glfwGetWindowPos(static_cast<GLFWwindow*>(native_window), &x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}

void LinuxWindow::maximize() { glfwMaximizeWindow(static_cast<GLFWwindow*>(native_window)); }

void LinuxWindow::center_window() {
    const auto& video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int x = (video_mode->width - static_cast<int>(window_data.width)) / 2;
    int y = (video_mode->height - static_cast<int>(window_data.height)) / 2;
    glfwSetWindowPos(static_cast<GLFWwindow*>(native_window), x, y);
}

void LinuxWindow::set_event_callback(const EventCallbackFn& callback) {
    window_data.event_callback = callback;
}

void LinuxWindow::set_resizable(const bool resizable) const {
    glfwSetWindowAttrib(static_cast<GLFWwindow*>(native_window),
        GLFW_RESIZABLE,
        resizable ? GLFW_TRUE : GLFW_FALSE);
}

const std::string& LinuxWindow::get_title() const { return window_data.title; }

void LinuxWindow::set_title(const std::string& title) {
    window_data.title = title;
    glfwSetWindowTitle(static_cast<GLFWwindow*>(native_window), window_data.title.c_str());
}

void LinuxWindow::shutdown() {
    glfwDestroyWindow(static_cast<GLFWwindow*>(native_window));
    --window_count;

    if (window_count == 0) {
        glfwTerminate();
    }
}

void LinuxWindow::set_callbacks() {
    // Set data pointer for callbacks
    glfwSetWindowUserPointer(static_cast<GLFWwindow*>(native_window), &window_data);

    glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(native_window),
        [](GLFWwindow* window, int width, int height) {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.width = static_cast<uint32_t>(width);
            data.height = static_cast<uint32_t>(height);

            WindowResized event;
            event.width = static_cast<uint32_t>(width);
            event.height = static_cast<uint32_t>(height);
            data.event_callback(event);
        });

    glfwSetWindowIconifyCallback(static_cast<GLFWwindow*>(native_window),
        [](GLFWwindow* window, int iconified) {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowIconified event;
            event.minimized = (iconified == 1);
            data.event_callback(event);
        });

    glfwSetWindowCloseCallback(static_cast<GLFWwindow*>(native_window), 
        [](GLFWwindow* window) {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowClosed event;
            data.event_callback(event);
        });

    glfwSetKeyCallback(static_cast<GLFWwindow*>(native_window),
        [](GLFWwindow* window,
            int key,
            [[maybe_unused]] int scancode,
            int action,
            [[maybe_unused]] int mods) {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action) {
            case GLFW_PRESS: {
                // TODO: Handle modifiers
                Pressed<Key> event;
                event.source.code = static_cast<KeyCode>(key);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE: {
                Released<Key> event;
                event.source.code = static_cast<KeyCode>(key);
                data.event_callback(event);
                break;
            }
            case GLFW_REPEAT: {
                Pressed<Key> event;
                event.source.code = static_cast<KeyCode>(key);
                data.event_callback(event);
                break;
            }
            }
        });

    glfwSetCharCallback(static_cast<GLFWwindow*>(native_window),
        [](GLFWwindow* window, unsigned int key) {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            Pressed<Key> event;
            event.source.code = static_cast<KeyCode>(key);
            data.event_callback(event);
        });

    glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(native_window),
        [](GLFWwindow* window,
            int button,
            int action,
            [[maybe_unused]] int mods) {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action) {
            case GLFW_PRESS: {
                Pressed<MouseButton> event;
                event.source.code = static_cast<MouseCode>(button);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE: {
                Released<MouseButton> event;
                event.source.code = static_cast<MouseCode>(button);
                data.event_callback(event);
                break;
            }
            }
        });

    glfwSetScrollCallback(static_cast<GLFWwindow*>(native_window),
        [](GLFWwindow* window, const double offset_x, const double offset_y) {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            Moved<MouseWheel> event;
            event.source.offset_x = static_cast<float>(offset_x);
            event.source.offset_y = static_cast<float>(offset_y);
            data.event_callback(event);
        });

    glfwSetCursorPosCallback(static_cast<GLFWwindow*>(native_window),
        [](GLFWwindow* window, const double pos_x, const double pos_y) {
            WindowData& data =
                *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            Moved<Mouse> event;
            event.source.x = static_cast<float>(pos_x);
            event.source.y = static_cast<float>(pos_y);
            data.event_callback(event);
        });

}

} // namespace pine

#endif
