#pragma once

#include <type_traits>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "pine/pch.hpp"
#include "pine/renderer/framebuffer.hpp"
#include "pine/utils/math.hpp"

namespace pine::gui
{

using WindowFlags = ImGuiWindowFlags;
using Style = ImGuiStyle;

WindowFlags configure_window_flags(const bool fullscreen);
void set_dark_theme(Style& style);

} // namespace pine::gui
