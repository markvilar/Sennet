#pragma once

#include <type_traits>

#include <imgui.h>

#include "pine/pch.hpp"

namespace pine::gui
{

using ConfigFlags = int;

enum ConfigFlagOptions : ConfigFlags
{
    NONE = 0,
    ENABLE_KEYBOARD = 1 << 0,
    ENABLE_GAMEPAD = 1 << 1,
    ENABLE_SET_MOUSE_POS = 1 << 2,
    NO_CAPTURE_KEYBOARD = 1 << 3,
    NO_MOUSE = 1 << 4,
    NO_MOUSE_CURSOR_CHANGE = 1 << 5,
    DOCKING_ENABLE = 1 << 6,

    // DOCKING FLAGS
    VIEWPORTS_ENABLE = 1 << 10,
    ENABLE_SCALE_VIEWPORTS = 1 << 14,
    ENABLE_SCALE_FONTS = 1 << 15,
    IS_SRGB = 1 << 20,
    IS_TOUCHSCREEN = 1 << 21,
};

} // namespace pine::gui
