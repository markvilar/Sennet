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

    // If func has a non-void return type, we return it after ending the window.
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

    // If func has a non-void return type, we return it after ending the window.
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

// TODO: Improve templating - Allow container to be vector. (C++20s std::span)
template <typename T, size_t N>
auto dropdown(const char* name, T* t,
    const std::array<std::pair<const char*, T>, N> options)
{
    static_assert(std::is_trivial<T>::value, "T must be trivial.");
    static_assert(std::is_trivially_copyable<T>::value,
        "T must be trivially copyable.");
    static_assert(std::is_copy_constructible_v<T>,
        "T must be copy constructible.");

    static typename std::array<std::pair<const char*, T>, N>::size_type
        label_index = 0;

    // Set label index based on the value of t.
    for (size_t index = 0; index < options.size(); index++)
    {
        if (*t == options[index].second)
        {
            label_index = index;
        }
    }

    // Render dropdown menu and update reference value.
    if (ImGui::BeginCombo(name, options[label_index].first, 0))
    {
        for (size_t index = 0; index < options.size(); index++)
        {
            const auto is_selected = (label_index == index);
            if (ImGui::Selectable(options[index].first, is_selected))
            {
                label_index = index;
                *t = options[index].second;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return *t;
}

WindowFlags configure_window_flags(const bool fullscreen);
void set_dark_theme(Style& style);

void help_marker(const char* desc);
void empty_space(const float width, const float height);

// TODO: Fix implicit template deduction. Literal types ruin template argument
// deduction.
template <typename T>
auto slider_scalar(const char* name, T* value, const T min_value,
    const T max_value)
{
    static_assert(std::is_scalar<T>::value, "T must be scalar");
    static_assert(std::is_integral<T>::value
            || std::is_floating_point<T>::value,
        "T must be integral or floating point.");

    ImGuiDataType type = ImGuiDataType_S8;
    if constexpr (std::is_same<T, int8_t>::value)
    {
        type = ImGuiDataType_S8;
    }
    else if constexpr (std::is_same<T, int16_t>::value)
    {
        type = ImGuiDataType_S16;
    }
    else if constexpr (std::is_same<T, int32_t>::value)
    {
        type = ImGuiDataType_S32;
    }
    else if constexpr (std::is_same<T, int64_t>::value)
    {
        type = ImGuiDataType_S64;
    }
    else if constexpr (std::is_same<T, uint8_t>::value)
    {
        type = ImGuiDataType_U8;
    }
    else if constexpr (std::is_same<T, uint16_t>::value)
    {
        type = ImGuiDataType_U16;
    }
    else if constexpr (std::is_same<T, uint32_t>::value)
    {
        type = ImGuiDataType_U32;
    }
    else if constexpr (std::is_same<T, uint64_t>::value)
    {
        type = ImGuiDataType_U64;
    }
    else if constexpr (std::is_same<T, float>::value)
    {
        type = ImGuiDataType_Float;
    }
    else if constexpr (std::is_same<T, double>::value)
    {
        type = ImGuiDataType_Double;
    }

    return ImGui::SliderScalar(name, type, value, &min_value, &max_value);
}

} // namespace pine::ui
