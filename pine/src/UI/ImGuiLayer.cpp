#include "Pine/Pch.hpp"
#include "Pine/UI/ImGuiLayer.hpp"

#include "Pine/Core/Application.hpp"
#include "Pine/UI/UI.hpp"

// TODO: TEMPORARY
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Pine
{

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer"), m_Time(0.0f) {}

void ImGuiLayer::OnAttach()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_None;
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();

    auto& app = Application::Get();
    auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnEvent(Event& e)
{
    if (m_BlockEvents)
    {
        auto& io = ImGui::GetIO();
        e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        e.Handled |=
            e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
}

void ImGuiLayer::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End()
{
    auto& io = ImGui::GetIO();
    auto& app = Application::Get();
    io.DisplaySize =
        ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace Pine
