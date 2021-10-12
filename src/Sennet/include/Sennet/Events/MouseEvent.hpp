#pragma once

#include "Sennet/Core/Input.hpp"
#include "Sennet/Events/Event.hpp"

namespace Sennet
{

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(const float x, const float y) : m_MouseX(x), m_MouseY(y) {}

    float GetX() const { return m_MouseX; }
    float GetY() const { return m_MouseY; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "[MouseMovedEvent] " << GetX() << ", " << GetY();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
    float m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(const float offsetX, const float offsetY)
        : m_OffsetX(offsetX), m_OffsetY(offsetY)
    {
    }

    float GetOffsetX() const { return m_OffsetX; }
    float GetOffsetY() const { return m_OffsetY; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "[MouseScrolledEvent] " << GetOffsetX() << ", " << GetOffsetY();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
    float m_OffsetX, m_OffsetY;
};

class MouseButtonEvent : public Event
{
public:
    inline MouseCode GetMouseButton() const { return m_Button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

protected:
    MouseButtonEvent(const MouseCode button) : m_Button(button) {}

    MouseCode m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "[MouseButtonPressedEvent] " << m_Button;
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

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "[MouseButtonReleasedEvent] " << m_Button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace Sennet
