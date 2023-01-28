#include "pine/gui/window.hpp"

#include <imgui.h>

namespace pine::gui
{
Window::Window(const std::string& name_) : name(name_) {}

void Window::update_state()
{
    position = {ImGui::GetWindowPos().x, ImGui::GetWindowPos().y};
    size = {ImGui::GetWindowSize().x, ImGui::GetWindowSize().y};
    focused = ImGui::IsWindowFocused();
    hovered = ImGui::IsWindowHovered();
}

void Window::begin() const
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    // NOTE: Constrain window to main viewport for now.
    ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_None);
    ImGui::PopStyleVar();
}

void Window::end() const { ImGui::End(); }

} // namespace pine::gui
