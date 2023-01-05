#pragma once

#include <type_traits>

#include <imgui.h>

#include "pine/pch.hpp"

namespace pine::gui
{

using WindowFlags = ImGuiWindowFlags;
using Style = ImGuiStyle;

WindowFlags configure_window_flags(const bool fullscreen);
void set_dark_theme(Style& style);

} // namespace pine::gui
