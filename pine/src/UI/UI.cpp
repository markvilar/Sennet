#include "Pine/UI/UI.hpp"

#include "Pine/Pch.hpp"

namespace Pine::UI
{

WindowFlags ConfigureWindowFlags(const bool fullscreen)
{
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
    if (fullscreen)
    {
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus;
    }
    return windowFlags;
}

void SetDarkTheme(Style& style)
{
    auto& colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);

    // Headers
    colors[ImGuiCol_Header] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);

    // Buttons
    colors[ImGuiCol_Button] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.3805f, 0.381f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.2805f, 0.281f, 1.0f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);

    // Resize Grip
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

    // Check Mark
    colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);

    // Slider
    colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
}

void ImGuiHelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void AddEmptySpace(const float width, const float height)
{
    ImGui::Dummy({width, height});
}

bool SliderScalar(const std::string& name, int8_t* value, const int8_t minValue,
    const int8_t maxValue)
{
    return ImGui::SliderScalar(name.c_str(),
        ImGuiDataType_S8,
        value,
        &minValue,
        &maxValue);
}

bool SliderScalar(const std::string& name, uint8_t* value,
    const uint8_t minValue, const uint8_t maxValue)
{
    return ImGui::SliderScalar(name.c_str(),
        ImGuiDataType_U8,
        value,
        &minValue,
        &maxValue);
}

bool SliderScalar(const std::string& name, int16_t* value,
    const int16_t minValue, const int16_t maxValue)
{
    return ImGui::SliderScalar(name.c_str(),
        ImGuiDataType_S16,
        value,
        &minValue,
        &maxValue);
}

bool SliderScalar(const std::string& name, uint16_t* value,
    const uint16_t minValue, const uint16_t maxValue)
{
    return ImGui::SliderScalar(name.c_str(),
        ImGuiDataType_U16,
        value,
        &minValue,
        &maxValue);
}

bool SliderScalar(const std::string& name, int32_t* value,
    const int32_t minValue, const int32_t maxValue)
{
    return ImGui::SliderScalar(name.c_str(),
        ImGuiDataType_S32,
        value,
        &minValue,
        &maxValue);
}

bool SliderScalar(const std::string& name, uint32_t* value,
    const uint32_t minValue, const uint32_t maxValue)
{
    return ImGui::SliderScalar(name.c_str(),
        ImGuiDataType_U32,
        value,
        &minValue,
        &maxValue);
}

bool SliderScalar(const std::string& name, int64_t* value,
    const int64_t minValue, const int64_t maxValue)
{
    return ImGui::SliderScalar(name.c_str(),
        ImGuiDataType_S64,
        value,
        &minValue,
        &maxValue);
}

bool SliderScalar(const std::string& name, uint64_t* value,
    const uint64_t minValue, const uint64_t maxValue)
{
    return ImGui::SliderScalar(name.c_str(),
        ImGuiDataType_U64,
        value,
        &minValue,
        &maxValue);
}

bool SliderScalar(const std::string& name, float* value, const float minValue,
    const float maxValue)
{
    return ImGui::SliderScalar(name.c_str(),
        ImGuiDataType_Float,
        value,
        &minValue,
        &maxValue);
}

bool SliderScalar(const std::string& name, double* value, const double minValue,
    const double maxValue)
{
    return ImGui::SliderScalar(name.c_str(),
        ImGuiDataType_Double,
        value,
        &minValue,
        &maxValue);
}

} // namespace Pine::UI
