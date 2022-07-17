#pragma once

#include <string>

#include "pine/core/input.hpp"
#include "pine/events/event.hpp"

namespace pine
{

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(const float x, const float y) 
        : m_mouse_x(x), m_mouse_y(y) {}

    float get_x() const { return m_mouse_x; }
    float get_y() const { return m_mouse_y; }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[MouseMovedEvent] " << get_x() << ", " << get_y();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
    float m_mouse_x, m_mouse_y;
};

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(const float offsetX, const float offsetY)
        : m_offset_x(offsetX), m_offset_y(offsetY)
    {
    }

    float get_offset_x() const { return m_offset_x; }
    float get_offset_y() const { return m_offset_y; }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[MouseScrolledEvent] " << get_offset_x() << ", " << get_offset_y();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
    float m_offset_x, m_offset_y;
};

class MouseButtonEvent : public Event
{
public:
    inline MouseCode GetMouseButton() const { return m_button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

protected:
    MouseButtonEvent(const MouseCode button) : m_button(button) {}

    MouseCode m_button;
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
        ss << "[MouseButtonPressedEvent] " << m_button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button)
    {
    }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[MouseButtonReleasedEvent] " << m_button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace pine
