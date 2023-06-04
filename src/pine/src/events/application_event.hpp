#pragma once

#include <cstdint>
#include <string>

#include "event.hpp"

namespace pine
{

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
