#pragma once

#include "pine/core/input.hpp"

namespace pine
{

class LinuxInputHandle : public InputHandle
{
public:
    LinuxInputHandle(const Window& window);
    virtual ~LinuxInputHandle() = default;

    virtual bool is_key_pressed(const KeyCode key) const override;
    virtual bool is_mouse_button_pressed(const MouseCode button) const override;

    virtual std::pair<float, float> get_mouse_position() const override;

private:
    const Window* window;
};

}
