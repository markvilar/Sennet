#if defined(PINE_PLATFORM_LINUX)
#include "Pine/Pch.hpp"
#include "Pine/Core/Input.hpp"

#include "Pine/Core/Application.hpp"

#include <GLFW/glfw3.h>

namespace Pine
{

bool Input::IsKeyPressed(KeyCode keyCode)
{
    auto window = static_cast<GLFWwindow*>(
        Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, static_cast<int32_t>(keyCode));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(MouseCode button)
{
    auto window = static_cast<GLFWwindow*>(
        Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
    return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition()
{
    auto window = static_cast<GLFWwindow*>(
        Application::Get().GetWindow().GetNativeWindow());
    double posX, posY;
    glfwGetCursorPos(window, &posX, &posY);
    return {(float)posX, (float)posY};
}

float Input::GetMouseX() { return GetMousePosition().first; }

float Input::GetMouseY() { return GetMousePosition().second; }

} // namespace Pine

#endif
