#pragma once

#include <cstdint>
#include <variant>
#include <string>

#include "pine/pch.hpp"
#include "pine/core/key_codes.hpp"
#include "pine/core/mouse_codes.hpp"

namespace pine
{

// ----------------------------------------------------------------------------
// Actions
// ----------------------------------------------------------------------------

template <typename Source>
struct Pressed
{
    constexpr static std::string_view name{"Pressed"};
    uint32_t repeat;
    Source source;
};

template <typename Source>
struct Released
{
    constexpr static std::string_view name{"Released"};
    Source source;
};

template <typename Source>
struct Moved
{
    constexpr static std::string_view name{"Moved"};
    Source source;
};

// ----------------------------------------------------------------------------
// Sources
// ----------------------------------------------------------------------------

struct Mouse
{
    constexpr static std::string_view name{"Mouse"};
    float coordinate_x;
    float coordinate_y;
};

struct MouseButton
{
    constexpr static std::string_view name{"MouseButton"};
    MouseCode code;
    Mouse mouse;
};

struct MouseWheel
{
    constexpr static std::string_view name{"MouseWheel"};
    float offset_x;
    float offset_y;
    Mouse mouse;
};

struct Key
{
    constexpr static std::string_view name{"Key"};
    bool alt;
    bool ctrl;
    bool system;
    bool shift;
    KeyCode code;
};

// ----------------------------------------------------------------------------
// Special events
// ----------------------------------------------------------------------------

struct WindowClosed
{
    constexpr static std::string_view name{"WindowClosed"};
};

struct WindowIconified
{
    constexpr static std::string_view name{"WindowIconified"};
    bool minimized;
};

struct WindowResized
{
    constexpr static std::string_view name{"WindowResized"};
    uint32_t width;
    uint32_t height;
};

struct TimeElapsed
{
    constexpr static std::string_view name{"TimeElapsed"};
};

// ----------------------------------------------------------------------------
// Event
// ----------------------------------------------------------------------------

using Event = std::variant<
    std::monostate,

    // Move events
    Moved<Mouse>,
    Moved<MouseWheel>,

    // Mouse button
    Pressed<MouseButton>,
    Released<MouseButton>,
    
    // Key
    Pressed<Key>,
    Released<Key>,
    
    // Special
    WindowClosed,
    WindowIconified,
    WindowResized,
    TimeElapsed
>;

} // namespace pine
