#pragma once
#include "Sennet/Pch.hpp"

#include "Sennet/UI/ImGuiImplGLFW.hpp"
#include "Sennet/UI/ImGuiImplOpenGL3.hpp"

#include "Sennet/Utils/Math.hpp"

#include "imgui.h"

#include <string>

namespace Sennet { namespace UI {

typedef ImGuiWindowFlags WindowFlags;
typedef ImGuiStyle Style;

template <typename Function>
void AddWindow(const std::string& name, const Vec2& position, const Vec2& size, 
    const Function func)
{
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiCond_Always);

    ImGui::Begin(name.c_str(), nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus);
    func();
    ImGui::End();
}

// TODO: Needs to be tested.
template <typename Function>
void AddMainMenuBar(const Function func)
{
    if (ImGui::BeginMainMenuBar())
    {
        func();
        ImGui::EndMainMenuBar();
    }
}

// TODO: Needs to be tested.
template <typename Function>
void AddMenuBar(const Function func)
{
    if (ImGui::BeginMenuBar())
    {
        func();
        ImGui::EndMenuBar();
    }
}

void SetDarkTheme(Style& style);

void ImGuiHelpMarker(const char* desc);

}
}
