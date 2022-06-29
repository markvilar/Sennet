#pragma once

#include "pine/core/common.hpp"
#include "pine/core/key_codes.hpp"
#include "pine/core/mouse_codes.hpp"

namespace pine
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

} // namespace pine
