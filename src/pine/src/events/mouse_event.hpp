#pragma once

#include <string>

#include "event.hpp"

#include "../core/input.hpp"

namespace pine
{

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(const float x, const float y)
        : coordinate_x(x),
          coordinate_y(y)
    {
    }

    float get_x() const { return coordinate_x; }
    float get_y() const { return coordinate_y; }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[MouseMovedEvent] " << get_x() << ", " << get_y();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
    float coordinate_x, coordinate_y;
};

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(const float offset_x_, const float offset_y_)
        : offset_x(offset_x_),
          offset_y(offset_y_)
    {
    }

    float get_offset_x() const { return offset_x; }
    float get_offset_y() const { return offset_y; }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[MouseScrolledEvent] " << get_offset_x() << ", "
           << get_offset_y();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
    float offset_x, offset_y;
};

class MouseButtonEvent : public Event
{
public:
    inline MouseCode GetMouseButton() const { return button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

protected:
    MouseButtonEvent(const MouseCode button_) : button(button_) {}

    MouseCode button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button)
    {
    }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[MouseButtonPressedEvent] " << button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(const MouseCode button_)
        : MouseButtonEvent(button_)
    {
    }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[MouseButtonReleasedEvent] " << button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace pine
