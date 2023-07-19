#if defined(PINE_PLATFORM_WINDOWS)
#include "pine/platform/windows/input.hpp"

#include <GLFW/glfw3.h>

#include "pine/pch.hpp"

namespace pine {

WindowsInputHandle::WindowsInputHandle(const Window& window_) {
    window = &window_;
}

bool WindowsInputHandle::is_key_pressed(const KeyCode key) const {
    auto native_window = static_cast<GLFWwindow*>(window->get_native_window());
    auto state = glfwGetKey(native_window, static_cast<int32_t>(key));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInputHandle::is_mouse_button_pressed(const MouseCode button) const {
    auto native_window = static_cast<GLFWwindow*>(window->get_native_window());
    auto state =
        glfwGetMouseButton(native_window, static_cast<int32_t>(button));
    return state == GLFW_PRESS;
}

std::pair<float, float> WindowsInputHandle::get_mouse_position() const {
    auto native_window = static_cast<GLFWwindow*>(window->get_native_window());
    double pos_x, pos_y;
    glfwGetCursorPos(native_window, &pos_x, &pos_y);
    return {static_cast<float>(pos_x), static_cast<float>(pos_y)};
}

} // namespace pine

#endif
