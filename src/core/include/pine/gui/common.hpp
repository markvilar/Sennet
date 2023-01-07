#pragma once

#include <type_traits>

#include <imgui.h>

#include "pine/pch.hpp"

namespace pine::gui
{

// Config system
using ConfigFlags = int;

ConfigFlags get_config_flags();
void set_config_flags(const ConfigFlags& config);

enum ConfigFlagOptions : int
{
    None                    = 0,
    NavEnableKeyboard       = 1 << 0,
    NavEnableGamepad        = 1 << 1,
    NavEnableSetMousePos    = 1 << 2,
    NavNoCaptureKeyboard    = 1 << 3,
    NoMouse                 = 1 << 4,
    NoMouseCursorChange     = 1 << 5,
    DockingEnable           = 1 << 6,

    ViewportsEnable         = 1 << 10,
    DpiEnableScaleViewports = 1 << 14,
    DpiEnableScaleFonts     = 1 << 15,
    IsSRGB                  = 1 << 20,
    IsTouchScreen           = 1 << 21,
};

// Window system
using WindowFlags = int;

WindowFlags configure_window_flags(const bool fullscreen);

// Style system
using Style = ImGuiStyle;

void set_dark_theme(Style& style);

} // namespace pine::gui
