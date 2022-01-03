#pragma once
#include "Sennet/Pch.hpp"

#include "Sennet/Renderer/Framebuffer.hpp"
#include "Sennet/UI/ImGuiImplGLFW.hpp"
#include "Sennet/UI/ImGuiImplOpenGL3.hpp"
#include "Sennet/Utils/Math.hpp"

#include "imgui.h"

#include <string>

namespace Sennet::UI
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
    const Vec2& size, const std::shared_ptr<Framebuffer>& framebuffer,
    const Function func)
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

    if (framebuffer)
    {
        auto textureID = framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID),
            ImVec2{size.x, size.y},
            ImVec2{0, 1},
            ImVec2{1, 0});
    }

    ImGui::End();
}

template <typename Function> auto AddMainMenuBar(const Function func)
{
    if (ImGui::BeginMainMenuBar())
    {
        func();
        ImGui::EndMainMenuBar();
    }
}

// TODO: Needs to be tested.
template <typename Function> auto AddMenuBar(const Function func)
{
    if (ImGui::BeginMenuBar())
    {
        func();
        ImGui::EndMenuBar();
    }
}

WindowFlags ConfigureWindowFlags(const bool fullscreen);

void SetDarkTheme(Style& style);

void ImGuiHelpMarker(const char* desc);

} // namespace Sennet::UI
