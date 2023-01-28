#pragma once

#include <string_view>

#include "pine/gui/common.hpp"
#include "pine/renderer/framebuffer.hpp"
#include "pine/utils/math.hpp"

namespace pine::gui
{
inline void empty_space(const float width, const float height)
{
    ImGui::Dummy({width, height});
}

inline void help_marker(const char* description, const float scale = 35.0f)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * scale);
        ImGui::TextUnformatted(description);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
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

inline auto render_dockspace(const char* name, const bool fullscreen = true)
{
    auto window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (fullscreen)
    {
        const auto& viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus;
    }

    const auto io = ImGui::GetIO();

    [[maybe_unused]] auto style = ImGui::GetStyle();

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    static bool dockspace_open = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(name, &dockspace_open, window_flags);
    ImGui::PopStyleVar();

    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    ImGui::End();
}

// FIXME: Improve templating - Allow container to be vector. (C++20s std::span)
template <typename T, size_t N>
auto dropdown(const char* name,
    T* t,
    const std::array<std::pair<const char*, T>, N> options)
{
    static_assert(std::is_trivial<T>::value, "T must be trivial.");
    static_assert(std::is_trivially_copyable<T>::value,
        "T must be trivially copyable.");
    static_assert(std::is_copy_constructible_v<T>,
        "T must be copy constructible.");

    static
        typename std::array<std::pair<const char*, T>, N>::size_type label_index
        = 0;

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

template <typename T>
constexpr auto get_internal_type()
{
    if constexpr (std::is_same<T, int8_t>::value)
    {
        return ImGuiDataType_S8;
    }
    else if constexpr (std::is_same<T, int16_t>::value)
    {
        return ImGuiDataType_S16;
    }
    else if constexpr (std::is_same<T, int32_t>::value)
    {
        return ImGuiDataType_S32;
    }
    else if constexpr (std::is_same<T, int64_t>::value)
    {
        return ImGuiDataType_S64;
    }
    else if constexpr (std::is_same<T, uint8_t>::value)
    {
        return ImGuiDataType_U8;
    }
    else if constexpr (std::is_same<T, uint16_t>::value)
    {
        return ImGuiDataType_U16;
    }
    else if constexpr (std::is_same<T, uint32_t>::value)
    {
        return ImGuiDataType_U32;
    }
    else if constexpr (std::is_same<T, uint64_t>::value)
    {
        return ImGuiDataType_U64;
    }
    else if constexpr (std::is_same<T, float>::value)
    {
        return ImGuiDataType_Float;
    }
    else if constexpr (std::is_same<T, double>::value)
    {
        return ImGuiDataType_Double;
    }
}

// FIXME: Fix implicit template deduction. Literal types ruin template argument
// deduction.
template <typename T>
auto slider_scalar(const char* name,
    T* value,
    const T min_value,
    const T max_value)
{
    static_assert(std::is_scalar<T>::value, "T must be scalar");
    static_assert(std::is_integral<T>::value
            || std::is_floating_point<T>::value,
        "T must be integral or floating point.");

    const auto type = get_internal_type<T>();
    ImGui::SliderScalar(name, type, value, &min_value, &max_value);
}

template <typename T>
auto input_scalar(const char* name, T* value, const char* format = nullptr)
{
    static_assert(std::is_scalar<T>::value, "T must be scalar");
    static_assert(std::is_integral<T>::value
            || std::is_floating_point<T>::value,
        "T must be integral or floating point.");

    const auto type = get_internal_type<T>();
    ImGui::InputScalar(name, type, value, nullptr, nullptr, format);
}

} // namespace pine::gui
