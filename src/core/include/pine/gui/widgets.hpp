#pragma once

#include "pine/gui/common.hpp"
#include "pine/renderer/framebuffer.hpp"
#include "pine/utils/math.hpp"


namespace pine::gui
{

struct PanelState
{
    Vec2 position;
    Vec2 size;
    bool focused;
    bool hovered;
};

inline PanelState get_panel_state()
{
    PanelState state;
    state.position = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
    state.size = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y } ;
    state.focused = ImGui::IsWindowFocused();
    state.hovered = ImGui::IsWindowHovered();

    return state;
}

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
            | ImGuiWindowFlags_NoCollapse 
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus;
    }

    const auto io = ImGui::GetIO();

    [[maybe_unused]]
    auto style = ImGui::GetStyle();

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

template <typename Function>
auto render_window(const char* name, const Function func)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
    ImGui::Begin(name, nullptr, 0);
    ImGui::PopStyleVar();
    func();

    const auto state = get_panel_state();

    ImGui::End();

    return state;
}

inline auto render_viewport(const char* name, const Framebuffer& framebuffer)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
    ImGui::Begin(name,
        nullptr,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleVar();

    const auto state = get_panel_state();
    const auto texture_id = framebuffer.get_color_attachment_renderer_id();

    ImGui::Image(reinterpret_cast<void*>(texture_id),
        ImVec2{state.size.x, state.size.y},
        ImVec2{0, 1},
        ImVec2{1, 0});

    ImGui::End();

    return state;
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

} // namespace pine::gui
