#if defined(PINE_WINDOW_GLFW)

#include "pine/platform/glfw/window.hpp"

#include <GLFW/glfw3.h>

namespace pine {

// ----------------------------------------------------------------------------
// ---- Statics ---------------------------------------------------------------
// ----------------------------------------------------------------------------

static void error_callback(const int error, const char* description) {
    PINE_CORE_ERROR("GLFW error ({0}): {1}", error, description);
}

// Statics to monitor the state of GLFW
static bool glfw_initialized = false;
static uint8_t glfw_window_count = 0;

// ----------------------------------------------------------------------------
// ---- Free functions / API wrappers -----------------------------------------
// ----------------------------------------------------------------------------

namespace glfw {

bool load() noexcept {
    glfwSetErrorCallback(error_callback);
    const auto success = glfwInit();
    if (success) {
        glfw_initialized = true;
    }
    return success;
}

bool loaded() noexcept {
    return glfw_initialized;
}

bool unload() noexcept {
    if (glfw_initialized) {
        glfwTerminate();
        glfw_initialized = false;
        return true;
    } else {
        return false;
    }
}

bool is_key_pressed(GLFWwindow* window, const KeyCode key) {
    auto state = glfwGetKey(window, static_cast<int>(key));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool is_mouse_button_pressed(GLFWwindow* window, const MouseCode button) {
    auto state = glfwGetMouseButton(window, static_cast<int>(button));
    return state == GLFW_PRESS;
}

std::pair<float, float> get_mouse_position(GLFWwindow* window) {
    double pos_x, pos_y;
    glfwGetCursorPos(window, &pos_x, &pos_y);
    return {static_cast<float>(pos_x), static_cast<float>(pos_y)};
}

} // namespace glfw

// ----------------------------------------------------------------------------
// ---- GlfwContext -----------------------------------------------------------
// ----------------------------------------------------------------------------

// Initialize statics
uint8_t GlfwContext::window_count = 0;
GlfwContext* GlfwContext::instance = nullptr;

void GlfwContext::poll_events() {
    glfwPollEvents();
}

void GlfwContext::set_vsync(const bool enabled) {
    if (enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
    vsync = enabled;
}

void GlfwContext::set_context(WindowType& window) {
    glfwMakeContextCurrent(static_cast<GLFWwindow*>(window.get_native()));
}

bool GlfwContext::has_context() {
    return glfwGetCurrentContext() != nullptr;
}

std::unique_ptr<GlfwContext::WindowType> GlfwContext::create_window(const WindowSpecs& specs) {
    window_count++;
    return std::make_unique<GlfwContext::WindowType>(specs);
}

void GlfwContext::init() {
    PINE_CORE_VERIFY(instance == nullptr, "GLFW context already exists!");
    instance = this;

    // Initialize GLFW
    if (window_count == 0) {
        [[maybe_unused]] const auto success = glfwInit();
        PINE_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(error_callback);
    }


#if defined(PINE_DEBUG)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    initialized = true;
}

void GlfwContext::shutdown() {
    if (window_count == 0) {
        glfwTerminate();
    }

    initialized = false;
    instance = nullptr;
}

// ----------------------------------------------------------------------------
// ---- GlfwInputHandle -------------------------------------------------------
// ----------------------------------------------------------------------------

GlfwInputHandle::GlfwInputHandle(GLFWwindow* window_) : window(window_) {}

bool GlfwInputHandle::is_key_pressed(const KeyCode key) {
    return glfw::is_key_pressed(window, key);
}

bool GlfwInputHandle::is_mouse_button_pressed(const MouseCode button) {
    return glfw::is_mouse_button_pressed(window, button);
}

std::pair<float, float> GlfwInputHandle::get_mouse_position() {
    return glfw::get_mouse_position(window);
}

// ----------------------------------------------------------------------------
// ---- GlfwWindow ------------------------------------------------------------
// ----------------------------------------------------------------------------

GlfwWindow::GlfwWindow(const WindowSpecs& specs) : specification(specs) { 
    init();
}

GlfwWindow::~GlfwWindow() { shutdown(); }

void GlfwWindow::init() {
    window_data.title = specification.title;
    window_data.width = specification.width;
    window_data.height = specification.height;

    // Create window
    window = glfwCreateWindow(
        static_cast<int>(window_data.width),
        static_cast<int>(window_data.height),
        window_data.title.c_str(),
        nullptr,
        nullptr);

    // Set up event callbacks
    set_callbacks();
}

void GlfwWindow::shutdown() {
    glfwDestroyWindow(window);
}

void GlfwWindow::swap_buffers() {
    glfwSwapBuffers(window);
}

std::string_view GlfwWindow::get_title() const {
    return window_data.title;
}

std::pair<uint32_t, uint32_t> GlfwWindow::get_size() const {
    return { window_data.width, window_data.height };
}

std::pair<float, float> GlfwWindow::get_position() const {
    int x, y;
    glfwGetWindowPos(window, &x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}

GlfwInputHandle GlfwWindow::get_input() const {
    return GlfwInputHandle(window);
}

void GlfwWindow::maximize() {
    glfwMaximizeWindow(window);
}

void GlfwWindow::center() {
    const auto& video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    auto x = (video_mode->width - static_cast<int>(window_data.width)) / 2;
    auto y = (video_mode->height - static_cast<int>(window_data.height)) / 2;
    glfwSetWindowPos(window, x, y);
}
    
void GlfwWindow::set_callbacks() {
    // Set internal data for access in callback functions
    set_callback_data();

    glfwSetWindowSizeCallback(window,
        [](GLFWwindow* window, int width_, int height_) {
            auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.width = static_cast<uint32_t>(width_);
            data.height = static_cast<uint32_t>(height_);

            WindowResized event;
            event.width = static_cast<uint32_t>(width_);
            event.height = static_cast<uint32_t>(height_);
            data.event_callback(event);
        });

    glfwSetWindowIconifyCallback(window,
        [](GLFWwindow* window, int iconified) {
            auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            const auto is_minimized = (iconified == 1);
            data.minimized = is_minimized;
            WindowIconified event;
            event.minimized = is_minimized;
            data.event_callback(event);
        });

    glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
        auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        WindowClosed event;
        data.event_callback(event);
    });

    glfwSetKeyCallback(window,
        [](GLFWwindow* window,
            int key,
            [[maybe_unused]] int scancode,
            int action,
            [[maybe_unused]] int mods) {
            auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
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

    glfwSetCharCallback(window,
        [](GLFWwindow* window, unsigned int key) {
            auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            Pressed<Key> event;
            event.source.code = static_cast<KeyCode>(key);
            data.event_callback(event);
        });

    glfwSetMouseButtonCallback(window,
        [](GLFWwindow* window,
            int button,
            int action,
            [[maybe_unused]] int mods) {
            auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action) {
            // TODO: Handle modifiers
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

    glfwSetScrollCallback(window,
        [](GLFWwindow* window, const double offset_x, const double offset_y) {
            auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            Moved<MouseWheel> event;
            event.source.offset_x = static_cast<float>(offset_x);
            event.source.offset_y = static_cast<float>(offset_y);
            data.event_callback(event);
        });

    glfwSetCursorPosCallback(window,
        [](GLFWwindow* window, const double pos_x, const double pos_y) {
            auto data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            Moved<Mouse> event;
            event.source.x = static_cast<float>(pos_x);
            event.source.y = static_cast<float>(pos_y);
            data.event_callback(event);
        });
}

void GlfwWindow::set_callback_data() {
    glfwSetWindowUserPointer(window, &window_data);
}

} // namespace pine

#endif
