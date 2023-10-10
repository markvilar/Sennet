#include "pine/platform/glfw/api.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "pine/platform/opengl/utilities.hpp"

namespace pine::glfw {

static void error_callback(const int error, const char* description) {
    PINE_CORE_ERROR("GLFW error ({0}): {1}", error, description);
}

/*
Loading functions:
- load
- loaded
- unload
*/

static bool glfw_initialized = false;

bool load() noexcept {
    glfwSetErrorCallback(error_callback);
    const auto success = glfwInit();
    if (success) {
        glfw_initialized = true;
    }
    return success;
}

bool is_loaded() noexcept {
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

/*
Input functions:
- is_key_pressed
- is_mouse_button_pressed
- get_mouse_position
- poll_events
*/

bool is_key_pressed(GLFWwindow* window, const KeyCode key) noexcept {
    auto state = glfwGetKey(window, static_cast<int>(key));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool is_mouse_button_pressed(GLFWwindow* window, const MouseCode button) noexcept {
    auto state = glfwGetMouseButton(window, static_cast<int>(button));
    return state == GLFW_PRESS;
}

std::pair<float, float> get_mouse_position(GLFWwindow* window) noexcept {
    double pos_x, pos_y;
    glfwGetCursorPos(window, &pos_x, &pos_y);
    return {static_cast<float>(pos_x), static_cast<float>(pos_y)};
}

void poll_events() noexcept {
    glfwPollEvents();
}

/*
Window functions:
- create_window
- set_context
- has_context
- set_vsync
*/

static uint8_t window_count = 0;

GLFWwindow* create_window(
    const uint32_t width, 
    const uint32_t height,
    const std::string& title
) noexcept {
    window_count++;
    return glfwCreateWindow(
        static_cast<int>(width),
        static_cast<int>(height),
        title.c_str(),
        nullptr,
        nullptr
    );
}

uint8_t get_window_count() noexcept {
    return window_count;
}

void set_context(GLFWwindow* window) noexcept {
    glfwMakeContextCurrent(window);
}

bool has_context() noexcept {
    return glfwGetCurrentContext() != nullptr;
}

void set_vsync(const bool enabled) noexcept {
    if (enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}

/*
Graphics loading functions:
- load_opengl
- opengl_loaded
*/
bool load_opengl() noexcept {
    const auto status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    PINE_CORE_VERIFY(status, "Failed to initialize Glad!");

    if (status) {
        PINE_CORE_INFO("Glad loader:");
        PINE_CORE_INFO(" - Vendor:   {0}", opengl::get_opengl_vendor());
        PINE_CORE_INFO(" - Renderer: {0}", opengl::get_opengl_renderer());
        PINE_CORE_INFO(" - Version:  {0}", opengl::get_opengl_version());
    }

    return status;
}

bool opengl_loaded() noexcept {
    return gladLoadGL();
}

}
