#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "pine/core/input.hpp"
#include "pine/core/window.hpp"

// Forward declaration
struct GLFWwindow;

namespace pine {

namespace glfw {

// Backend loading
bool load() noexcept;
bool loaded() noexcept;
bool unload() noexcept;

// Input functions
bool is_key_pressed(GLFWwindow* window, const KeyCode key);
bool is_mouse_button_pressed(GLFWwindow* window, const MouseCode button);
std::pair<float, float> get_mouse_position(GLFWwindow* window);

} // namespace glfw

// Forward declaration
class GlfwWindow;
class GlfwInput;

class GlfwContext {
public:
    using WindowType = GlfwWindow;
    using InputType = bool; // NOTE: Temporary

    GlfwContext() = default;
    ~GlfwContext() = default;

    void poll_events();

    void set_vsync(const bool enabled);
    void set_context(WindowType& window);

    bool has_context();

    // Factories
    std::unique_ptr<WindowType> create_window(const WindowSpecs& specs = {});

private:
    void init();
    void shutdown();

private:
    bool initialized{false};
    bool vsync{false};
    static uint8_t window_count;
    static GlfwContext* instance;
};

class GlfwInputHandle {
public:
    GlfwInputHandle(GLFWwindow* window);
    ~GlfwInputHandle() = default;

    bool is_key_pressed(const KeyCode key);
    bool is_mouse_button_pressed(const MouseCode button);
    std::pair<float, float> get_mouse_position();

private:
    GLFWwindow* window;
};

class GlfwWindow {

public:
    using InputType = GlfwInputHandle;

    GlfwWindow(const WindowSpecs& specs = {});
    ~GlfwWindow();
    
    void swap_buffers();

    std::string_view get_title() const;
    std::pair<uint32_t, uint32_t> get_size() const;
    std::pair<float, float> get_position() const;
    GlfwInputHandle get_input() const;
    
    void maximize();
    void center();

    template <EventCallback Callback>
    void set_event_callback(const Callback& callback) {
        window_data.event_callback = callback;
    }

    WindowSpecs get_specification() { return specification; }
    void* get_native() { return reinterpret_cast<void*>(window); }

private:
    using NativeWindow = GLFWwindow;

    void init();
    void shutdown();

    void set_callbacks();
    void set_callback_data();

    struct WindowData {
        std::string title;
        uint32_t width;
        uint32_t height;
        bool minimized;
        std::function<void(const Event&)> event_callback = [](const Event&) {};
    };

private:
    NativeWindow* window;
    WindowSpecs specification;
    WindowData window_data;

    // TODO: Add reference to context
};

} // namespace pine
