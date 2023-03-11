#if defined(PINE_PLATFORM_WINDOWS)
#include "pine/core/input.hpp"

#include <GLFW/glfw3.h>

#include "pine/pch.hpp"

namespace pine::input
{

bool is_key_pressed(const Window& window, const KeyCode key)
{
    const auto native_window
        = static_cast<GLFWwindow*>(window.get_native_window());
    const auto state = glfwGetKey(native_window, static_cast<int32_t>(key));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool is_mouse_button_pressed(const Window& window, const MouseCode button)
{
    const auto native_window
        = static_cast<GLFWwindow*>(window.get_native_window());
    const auto state
        = glfwGetMouseButton(native_window, static_cast<int32_t>(button));
    return state == GLFW_PRESS;
}

std::pair<float, float> get_mouse_position(const Window& window)
{
    auto native_window = static_cast<GLFWwindow*>(window.get_native_window());
    double posx, posy;
    glfwGetCursorPos(native_window, &posx, &posy);
    return {(float)posx, (float)posy};
}

float get_mouse_x(const Window& window)
{
    return get_mouse_position(window).first;
}

float get_mouse_y(const Window& window);
{
    return get_mouse_position(window).second;
}

} // namespace pine::input

#endif
