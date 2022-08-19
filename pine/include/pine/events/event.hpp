#pragma once
#include "pine/pch.hpp"

#include "pine/core/common.hpp"

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
    static EventType get_static_type()                                         \
    {                                                                          \
        return EventType::type;                                                \
    }                                                                          \
    virtual EventType get_event_type() const override                          \
    {                                                                          \
        return get_static_type();                                              \
    }                                                                          \
    virtual const char* get_name() const override                              \
    {                                                                          \
        return #type;                                                          \
    }

#define EVENT_CLASS_CATEGORY(category)                                         \
    virtual int get_category_flags() const override                            \
    {                                                                          \
        return category;                                                       \
    }

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
    EventDispatcher(Event& event) : m_event(event) {}

    template <typename T, typename F>
    bool dispatch(const F& func)
    {
        if (m_event.get_event_type() == T::get_static_type())
        {
            m_event.handled = func(static_cast<T&>(m_event));
            return true;
        }
        return false;
    }

private:
    Event& m_event;
};

inline std::ostream& operator<<(std::ostream& os, Event& event)
{
    return os << event.to_string();
}

} // namespace pine
