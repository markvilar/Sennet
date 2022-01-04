#pragma once

#include "Pine/Core/Base.hpp"
#include "Pine/Core/KeyCodes.hpp"
#include "Pine/Core/MouseCodes.hpp"

namespace Pine
{

class Input
{
public:
    static bool IsKeyPressed(KeyCode keyCode);

    static bool IsMouseButtonPressed(MouseCode button);
    static std::pair<float, float> GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};

} // namespace Pine
