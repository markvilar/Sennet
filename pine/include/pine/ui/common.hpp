#pragma once

#include <type_traits>

#include <imgui.h>

#include "pine/pch.hpp"
#include "pine/renderer/framebuffer.hpp"
#include "pine/ui/imgui_impl_glfw.hpp"
#include "pine/ui/imgui_impl_opengl3.hpp"
#include "pine/utils/math.hpp"

namespace pine::ui
{

typedef ImGuiWindowFlags WindowFlags;
typedef ImGuiStyle Style;

template <typename Function>
auto render_window(const char* name, const Vec2& position, const Vec2& size,
    const Function func)
{
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiCond_Always);

    ImGui::Begin(name,
        nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus);

    if constexpr (!std::is_void<decltype(func())>::value)
    {
        const auto value = func();
        ImGui::End();
        return value;
    }
    else
    {
        func();
        ImGui::End();
    }
}

template <typename Function>
auto render_viewport(const char* name, const Vec2& position, const Vec2& size,
    const Framebuffer& framebuffer, const Function func)
{
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiCond_Always);

    ImGui::Begin(name,
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoScrollbar
            | ImGuiWindowFlags_NoScrollWithMouse);

    // If func has a return type, we return it after ending the window.
    if constexpr (!std::is_void<decltype(func())>::value)
    {
        const auto value = func();
        const auto textureID = framebuffer.get_color_attachment_renderer_id();
        ImGui::Image(reinterpret_cast<void*>(textureID),
            ImVec2{size.x, size.y},
            ImVec2{0, 1},
            ImVec2{1, 0});
        ImGui::End();
        return value;
    }
    else
    {
        func();
        const auto textureID = framebuffer.get_color_attachment_renderer_id();
        ImGui::Image(reinterpret_cast<void*>(textureID),
            ImVec2{size.x, size.y},
            ImVec2{0, 1},
            ImVec2{1, 0});
        ImGui::End();
    }
}

template <typename Function>
auto main_menu_bar(const Function func)
{
    if (ImGui::BeginMainMenuBar())
    {
        func();
        ImGui::EndMainMenuBar();
    }
}

template <typename Function>
auto menu_bar(const Function func)
{
    if (ImGui::BeginMenuBar())
    {
        func();
        ImGui::EndMenuBar();
    }
}

// TODO: Better templating - Make container type more generic.
template <typename T, size_t size>
void dropdown(const char* name, T* value,
    const std::array<std::pair<const char*, T>, size>& options)
{
    static size_t label_index = 0;

    const auto label = options[label_index].first;
    if (ImGui::BeginCombo(name, label, 0))
    {
        for (size_t index = 0; index < options.size(); index++)
        {
            const auto is_selected = (label_index == index);
            if (ImGui::Selectable(options[index].first, is_selected))
            {
                label_index = index;
                *value = options[index].second;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

WindowFlags configure_window_flags(const bool fullscreen);
void set_dark_theme(Style& style);

void help_marker(const char* desc);
void empty_space(const float width, const float height);

bool slider_scalar(const char* name, int8_t* value, const int8_t min_value,
    const int8_t max_value);
bool slider_scalar(const char* name, uint8_t* value, const uint8_t min_value,
    const uint8_t max_value);
bool slider_scalar(const char* name, int16_t* value, const int16_t min_value,
    const int16_t max_value);
bool slider_scalar(const char* name, uint16_t* value, const uint16_t min_value,
    const uint16_t max_value);
bool slider_scalar(const char* name, int32_t* value, const int32_t min_value,
    const int32_t max_value);
bool slider_scalar(const char* name, uint32_t* value, const uint32_t min_value,
    const uint32_t max_value);
bool slider_scalar(const char* name, int64_t* value, const int64_t min_value,
    const int64_t max_value);
bool slider_scalar(const char* name, uint64_t* value, const uint64_t min_value,
    const uint64_t max_value);
bool slider_scalar(const char* name, float* value, const float min_value,
    const float max_value);
bool slider_scalar(const char* name, double* value, const double min_value,
    const double max_value);

} // namespace pine::ui
