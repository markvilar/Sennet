#pragma once

#include "pine/core/common.hpp"
#include "pine/core/key_codes.hpp"
#include "pine/core/mouse_codes.hpp"
#include "pine/core/window.hpp"

namespace pine::input
{

class InputHandle
{
    /* Interface for input handles. */

public:
    virtual ~InputHandle() = default;

    virtual bool is_key_pressed(const KeyCode key) = 0;
    virtual bool is_mouse_button_pressed(const MouseCode button) = 0;

    virtual std::pair<float, float> get_mouse_position() = 0;

    // TODO: Implement.
    static std::unique_ptr<InputHandle> create() { return nullptr; }
};

bool is_key_pressed(const Window& window, const KeyCode key);
bool is_mouse_button_pressed(const Window& window, const MouseCode button);

std::pair<float, float> get_mouse_position(const Window& window);

// TODO: Remove.
float get_mouse_x(const Window& window);
float get_mouse_y(const Window& window);

} // namespace pine::input
