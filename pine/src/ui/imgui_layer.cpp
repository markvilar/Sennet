#include "pine/ui/imgui_layer.hpp"

// TODO: TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "pine/core/application.hpp"
#include "pine/pch.hpp"
#include "pine/ui/common.hpp"

namespace pine
{

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer"), m_Time(0.0f) {}

void ImGuiLayer::on_attach()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_None;
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();

    auto& app = Application::get();
    auto window = static_cast<GLFWwindow*>(app.get_window().get_native_window());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::on_detach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::on_event(Event& event)
{
    if (m_BlockEvents)
    {
        auto& io = ImGui::GetIO();
        event.Handled |= event.IsInCategory(EventCategoryMouse) 
            & io.WantCaptureMouse;
        event.Handled |= event.IsInCategory(EventCategoryKeyboard) 
            & io.WantCaptureKeyboard;
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
    auto& app = Application::get();
    io.DisplaySize =
        ImVec2(app.get_window().get_width(), app.get_window().get_height());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace pine
