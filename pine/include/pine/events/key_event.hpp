#pragma once

#include <sstream>
#include <string>

#include "pine/core/input.hpp"
#include "pine/events/event.hpp"

namespace pine
{

class KeyEvent : public Event
{
public:
    KeyCode GetKeyCode() const { return m_key_code; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
    KeyEvent(const KeyCode key_code) : m_key_code(key_code) {}

    KeyCode m_key_code;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(const KeyCode key_code, const int repeat_count)
        : KeyEvent(key_code), m_repeat_count(repeat_count)
    {
    }

    int GetRepeatCount() const { return m_repeat_count; }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[KeyPressedEvent] " << m_key_code << " (" << m_repeat_count
           << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)

private:
    int m_repeat_count;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(const KeyCode key_code) : KeyEvent(key_code) {}

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "[KeyReleasedEvent] " << m_key_code;
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
        ss << "[KeyTypedEvent] " << m_key_code;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};

} // namespace pine
