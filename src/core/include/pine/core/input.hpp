#pragma once

#include "pine/core/common.hpp"
#include "pine/core/key_codes.hpp"
#include "pine/core/mouse_codes.hpp"
#include "pine/core/window.hpp"

namespace pine
{
class InputHandle
{
    /* Interface for input handles. */

public:
    virtual ~InputHandle() = default;

    virtual bool is_key_pressed(const KeyCode key) const = 0;
    virtual bool is_mouse_button_pressed(const MouseCode button) const = 0;

    virtual std::pair<float, float> get_mouse_position() const = 0;

    static std::unique_ptr<InputHandle> create(const Window& window);
};

} // namespace pine
