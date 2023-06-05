#pragma once

#include <cstdint>
#include <string>
#include <sstream>

#include "pine/pch.hpp"
#include "pine/core/key_codes.hpp"
#include "pine/core/mouse_codes.hpp"
#include "pine/defines/common.hpp"

namespace pine
{

enum class EventType
{
    None = 0,
    WindowClose,
    WindowResize,
    WindowIconify,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    AppTick,
    AppUpdate,
    AppRender,
    KeyPressed,
    KeyReleased,
    KeyTyped,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

enum EventCategory
{
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3),
    EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                 \
    static EventType get_static_type() { return EventType::type; }             \
    virtual EventType get_event_type() const override                          \
    {                                                                          \
        return get_static_type();                                              \
    }                                                                          \
    virtual const char* get_name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)                                         \
    virtual int get_category_flags() const override { return category; }

class Event
{
public:
    virtual ~Event() = default;

    virtual EventType get_event_type() const = 0;
    virtual const char* get_name() const = 0;
    virtual int get_category_flags() const = 0;
    virtual std::string to_string() const { return get_name(); }

    bool is_in_category(const EventCategory& category) const
    {
        return get_category_flags() & category;
    }

public:
    mutable bool handled = false;
};

template <typename T, typename Callable>
bool dispatch_event(const Event& event, const Callable& callable)
{
    if (event.get_event_type() == T::get_static_type())
    {
        event.handled = callable(static_cast<T&>(event));
        return true;
    }
    return false;
}

class EventDispatcher
{
public:
    EventDispatcher(Event& event_) : event(event_) {}

    template <typename T, typename F>
    bool dispatch(const F& func)
    {
        if (event.get_event_type() == T::get_static_type())
        {
            event.handled = func(static_cast<T&>(event));
            return true;
        }
        return false;
    }

private:
    Event& event;
};

inline std::ostream& operator<<(std::ostream& os, Event& event)
{
    return os << event.to_string();
}

// ----------------------------------------------------------------------------
// Application event
// ----------------------------------------------------------------------------

class AppUpdateEvent : public Event
{
public:
    AppUpdateEvent() = default;

    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event
{
public:
    AppRenderEvent() = default;

    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

// ----------------------------------------------------------------------------
// Window event
// ----------------------------------------------------------------------------

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(unsigned int width_, unsigned int height_)
        : width(width_),
          height(height_)
    {
    }

    inline uint32_t get_width() const { return width; }
    inline uint32_t get_height() const { return height; }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[WindowResizeEvent] " << width << ", " << height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
    uint32_t width;
    uint32_t height;
};

class WindowIconifyEvent : public Event
{
public:
    WindowIconifyEvent(const bool minimized_) : minimized(minimized_) {}

    bool is_minimized() const { return minimized; }

    EVENT_CLASS_TYPE(WindowIconify)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
    bool minimized;
};

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

// ----------------------------------------------------------------------------
// Key events
// ----------------------------------------------------------------------------

class KeyEvent : public Event
{
public:
    KeyCode GetKeyCode() const { return key_code; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
    KeyEvent(const KeyCode key_code) : key_code(key_code) {}

    KeyCode key_code;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(const KeyCode key_code, const int repeat_count_)
        : KeyEvent(key_code),
          repeat_count(repeat_count_)
    {
    }

    int GetRepeatCount() const { return repeat_count; }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[KeyPressedEvent] " << key_code << " (" << repeat_count
           << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)

private:
    int repeat_count;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(const KeyCode key_code) : KeyEvent(key_code) {}

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[KeyReleasedEvent] " << key_code;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
    KeyTypedEvent(const KeyCode key_code) : KeyEvent(key_code) {}

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[KeyTypedEvent] " << key_code;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};

// ----------------------------------------------------------------------------
// Mouse events
// ----------------------------------------------------------------------------

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
