#pragma once

#include <cstdint>
#include <string>

#include "pine/events/event.hpp"

namespace pine
{

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_width(width), m_height(height)
    {
    }

    inline uint32_t get_width() const { return m_width; }
    inline uint32_t get_height() const { return m_height; }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[WindowResizeEvent] " << m_width << ", " << m_height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
    uint32_t m_width;
    uint32_t m_height;
};

class WindowIconifyEvent : public Event
{
public:
    WindowIconifyEvent(bool minimized) : m_minimized(minimized) {}

    bool is_minimized() const { return m_minimized; }

    EVENT_CLASS_TYPE(WindowIconify)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
    bool m_minimized;
};

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

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

} // namespace pine
