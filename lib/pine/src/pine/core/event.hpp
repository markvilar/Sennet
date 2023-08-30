#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <variant>

#include "pine/core/codes.hpp"
#include "pine/pch.hpp"

namespace pine {

// ----------------------------------------------------------------------------
// Actions
// ----------------------------------------------------------------------------

template <typename Source>
struct Pressed {
    constexpr static std::string_view name{"Pressed"};
    constexpr static auto elements =
        std::to_array<std::string_view>({"repeat", "source"});
    uint32_t repeat;
    Source source;
};

template <typename Source>
struct Released {
    constexpr static std::string_view name{"Released"};
    constexpr static auto elements =
        std::to_array<std::string_view>({"source"});
    Source source;
};

template <typename Source>
struct Moved {
    constexpr static std::string_view name{"Moved"};
    constexpr static auto elements =
        std::to_array<std::string_view>({"source"});
    Source source;
};

// ----------------------------------------------------------------------------
// Sources
// ----------------------------------------------------------------------------

struct Mouse {
    constexpr static std::string_view name{"Mouse"};
    constexpr static auto elements =
        std::to_array<std::string_view>({"x", "y"});
    float x;
    float y;
};

struct MouseButton {
    constexpr static std::string_view name{"MouseButton"};
    constexpr static auto elements =
        std::to_array<std::string_view>({"code", "mouse"});
    MouseCode code;
    Mouse mouse;
};

struct MouseWheel {
    constexpr static std::string_view name{"MouseWheel"};
    constexpr static auto elements =
        std::to_array<std::string_view>({"offset_x", "offset_y", "mouse"});
    float offset_x;
    float offset_y;
    Mouse mouse;
};

struct Key {
    constexpr static std::string_view name{"Key"};
    constexpr static auto elements = std::to_array<std::string_view>(
        {"alt", "ctrl", "system", "shift", "code"});
    bool alt;
    bool ctrl;
    bool system;
    bool shift;
    KeyCode code;
};

// ----------------------------------------------------------------------------
// Special events
// ----------------------------------------------------------------------------

struct WindowClosed {
    constexpr static std::string_view name{"WindowClosed"};
    constexpr static std::array<std::string_view, 0> elements{};
};

struct WindowIconified {
    constexpr static std::string_view name{"WindowIconified"};
    constexpr static auto elements =
        std::to_array<std::string_view>({"minimized"});
    bool minimized;
};

struct WindowResized {
    constexpr static std::string_view name{"WindowResized"};
    constexpr static auto elements =
        std::to_array<std::string_view>({"width", "height"});
    uint32_t width;
    uint32_t height;
};

struct TimeElapsed {
    constexpr static std::string_view name{"TimeElapsed"};
    constexpr static std::array<std::string_view, 0> elements{};
};

// ----------------------------------------------------------------------------
// Event
// ----------------------------------------------------------------------------

// Event aliases
using MouseMoved = Moved<Mouse>;
using MouseWheelMoved = Moved<MouseWheel>;
using MousePressed = Pressed<MouseButton>;
using MouseReleased = Released<MouseButton>;
using KeyPressed = Pressed<Key>; 
using KeyReleased = Released<Key>;

using Event = std::variant<std::monostate,
    // Move events
    MouseMoved, MouseWheelMoved,
    // Mouse button
    MousePressed, MouseReleased,
    // Key
    KeyPressed, KeyReleased,
    // Special
    WindowClosed, WindowIconified, WindowResized, TimeElapsed>;

template <typename T, typename Callback>
auto dispatch_event(const Event& event, Callback callback) {
    if (std::holds_alternative<T>(event)) {
        callback(std::get<T>(event));
    }
}

} // namespace pine
