#include "pine/gui/manager.hpp"

// FIXME: GUI implementation abstraction
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "pine/pch.hpp"
#include "pine/gui/common.hpp"

namespace pine::gui
{

std::unique_ptr<Context> create_context(Window* window)
{
    return std::make_unique<Context>(window);
}

std::unique_ptr<Manager> create_manager(Window* window)
{
    auto context = create_context(window);
    return std::make_unique<Manager>(context);
}

Context::Context(Window* window_)
    : window(window_)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
}

Context::~Context()
{
    ImGui::DestroyContext();
}

void Context::init() const
{
    auto native_window = static_cast<GLFWwindow*>(window->get_native_window());
    ImGui_ImplGlfw_InitForOpenGL(native_window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void Context::shutdown() const
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}

void Context::begin_frame() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Context::end_frame() const
{
    auto& io = ImGui::GetIO();

    if (window)
    {
        io.DisplaySize = ImVec2(static_cast<float>(window->get_width()),
            static_cast<float>(window->get_height()));
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


Manager::Manager(std::unique_ptr<Context>& context_)
    : context(std::move(context_))
{
    auto config_flags = gui::get_config_flags();
    config_flags |= ConfigFlagOptions::NavEnableKeyboard;
    config_flags |= ConfigFlagOptions::DockingEnable;
    config_flags |= ConfigFlagOptions::ViewportsEnable;
    gui::set_config_flags(config_flags);
    
    // Set dark theme by default
    auto& style = ImGui::GetStyle();
    set_dark_theme(style);

    if (context)
    {
        context->init();
    }
    
    auto& io = ImGui::GetIO();
    io.IniFilename = profile.c_str(); 
}

Manager::~Manager()
{
    if (context)
    {
        context->shutdown();
        context.reset();
    }
}

void Manager::begin_frame() const
{
    if (context)
    {
        context->begin_frame();
    }
}

void Manager::end_frame() const
{
    if (context)
    {
        context->end_frame();
    }
}

void Manager::on_event(Event& event) const
{
    if (handle_event)
    {
        auto& io = ImGui::GetIO();
        event.handled |= event.is_in_category(EventCategoryMouse) 
            & io.WantCaptureMouse;
        event.handled |= event.is_in_category(EventCategoryKeyboard)
            & io.WantCaptureKeyboard;
    }
}

std::string Manager::get_profile_name() const
{
    return profile;
}

void Manager::set_profile_name(const std::string& name)
{
    profile = name;
    auto& io = ImGui::GetIO();
    io.IniFilename = profile.c_str(); 
}

} // namespace pine::gui
