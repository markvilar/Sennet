#pragma once

#include <sstream>
#include <string>

#include "event.hpp"

#include "../core/input.hpp"

namespace pine
{

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

} // namespace pine
