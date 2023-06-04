#pragma once

#include "../../core/input.hpp"

namespace pine
{

class WindowsInputHandle : public InputHandle
{
public:
    WindowsInputHandle(const Window& window);
    virtual ~WindowsInputHandle() = default;

    virtual bool is_key_pressed(const KeyCode key) const override;
    virtual bool is_mouse_button_pressed(const MouseCode button) const override;

    virtual std::pair<float, float> get_mouse_position() const override;

private:
    const Window* window;
};

} // namespace pine
