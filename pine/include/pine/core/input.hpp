#pragma once

#include "pine/core/common.hpp"
#include "pine/core/key_codes.hpp"
#include "pine/core/mouse_codes.hpp"
#include "pine/core/window.hpp"

namespace pine::input
{

bool is_key_pressed(const Window& window, const KeyCode key);
bool is_mouse_button_pressed(const Window& window, const MouseCode button);

std::pair<float, float> get_mouse_position(const Window& window);
float get_mouse_x(const Window& window);
float get_mouse_y(const Window& window);

} // namespace pine::input
