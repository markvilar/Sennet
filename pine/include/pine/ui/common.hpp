#pragma once

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
auto AddWindow(const std::string& name, const Vec2& position, const Vec2& size,
    const Function func)
{
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiCond_Always);

    ImGui::Begin(name.c_str(),
        nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus);
    func();
    ImGui::End();
}

// TODO: Make template able to return value from func.
template <typename Function>
auto AddViewport(const std::string& name, const Vec2& position,
    const Vec2& size, const Framebuffer& framebuffer, const Function func)
{
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiCond_Always);

    ImGui::Begin(name.c_str(),
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoScrollbar
            | ImGuiWindowFlags_NoScrollWithMouse);

    func();

    auto textureID = framebuffer.GetColorAttachmentRendererID();
    ImGui::Image(reinterpret_cast<void*>(textureID),
        ImVec2{size.x, size.y},
        ImVec2{0, 1},
        ImVec2{1, 0});

    ImGui::End();
}

template <typename Function>
auto AddMainMenuBar(const Function func)
{
    if (ImGui::BeginMainMenuBar())
    {
        func();
        ImGui::EndMainMenuBar();
    }
}

template <typename Function>
auto AddMenuBar(const Function func)
{
    if (ImGui::BeginMenuBar())
    {
        func();
        ImGui::EndMenuBar();
    }
}

template <typename T, size_t size>
void AddCombo(const std::string& name, T* value,
    const std::array<std::pair<std::string, T>, size>& options)
{
    static uint32_t labelIndex = 0;
    const auto label = options[labelIndex].first;
    if (ImGui::BeginCombo(name.c_str(), label.c_str(), 0))
    {
        for (auto index = 0; index < options.size(); index++)
        {
            const auto isSelected = (labelIndex == index);
            if (ImGui::Selectable(options[index].first.c_str(), isSelected))
            {
                labelIndex = index;
                *value = options[index].second;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

WindowFlags ConfigureWindowFlags(const bool fullscreen);
void SetDarkTheme(Style& style);
void ImGuiHelpMarker(const char* desc);

void AddEmptySpace(const float width, const float height);

bool SliderScalar(const std::string& name, int8_t* value, const int8_t minValue,
    const int8_t maxValue);

bool SliderScalar(const std::string& name, uint8_t* value,
    const uint8_t minValue, const uint8_t maxValue);

bool SliderScalar(const std::string& name, int16_t* value,
    const int16_t minValue, const int16_t maxValue);

bool SliderScalar(const std::string& name, uint16_t* value,
    const uint16_t minValue, const uint16_t maxValue);

bool SliderScalar(const std::string& name, int32_t* value,
    const int32_t minValue, const int32_t maxValue);

bool SliderScalar(const std::string& name, uint32_t* value,
    const uint32_t minValue, const uint32_t maxValue);

bool SliderScalar(const std::string& name, int64_t* value,
    const int64_t minValue, const int64_t maxValue);

bool SliderScalar(const std::string& name, uint64_t* value,
    const uint64_t minValue, const uint64_t maxValue);

bool SliderScalar(const std::string& name, float* value, const float minValue,
    const float maxValue);

bool SliderScalar(const std::string& name, double* value, const double minValue,
    const double maxValue);

} // namespace pine::ui
