#pragma once

#include "Sennet/Core/Base.hpp"
#include "Sennet/Core/KeyCodes.hpp"
#include "Sennet/Core/MouseCodes.hpp"

namespace Sennet
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

} // namespace Sennet
