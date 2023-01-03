#include "pine/gui/graphical_interface.hpp"

// ImGui
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// ImPlot
#include <implot.h>

// TODO: TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "pine/core/application.hpp"
#include "pine/pch.hpp"

namespace pine
{

std::unique_ptr<GraphicalInterface> GraphicalInterface::create(
    Window* window)
{
    return std::make_unique<GraphicalInterface>(window);
}

GraphicalInterface::GraphicalInterface(Window* window)
    : m_window(window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.IniFilename = nullptr; // TODO: Add user profile.

    ImGui::StyleColorsDark();

    auto native_window = static_cast<GLFWwindow*>(window->get_native_window());

    ImGui_ImplGlfw_InitForOpenGL(native_window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

GraphicalInterface::~GraphicalInterface()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void GraphicalInterface::begin_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GraphicalInterface::end_frame()
{
    auto& io = ImGui::GetIO();

    if (m_window)
    {
        io.DisplaySize = ImVec2(
            static_cast<float>(m_window->get_width()),
            static_cast<float>(m_window->get_height())
            );
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        auto backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void GraphicalInterface::on_event(Event& event)
{
    if (m_block_events)
    {
        auto& io = ImGui::GetIO();
        event.handled |=
            event.is_in_category(EventCategoryMouse) & io.WantCaptureMouse;
        event.handled |= event.is_in_category(EventCategoryKeyboard)
            & io.WantCaptureKeyboard;
    }
}

} // namespace pine
