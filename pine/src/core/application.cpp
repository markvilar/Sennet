#include "pine/core/application.hpp"

#include <GLFW/glfw3.h>

#include "pine/core/input.hpp"
#include "pine/core/log.hpp"
#include "pine/core/timestep.hpp"
#include "pine/pch.hpp"
#include "pine/renderer/renderer.hpp"

namespace pine
{

Application* Application::s_instance = nullptr;

Application::Application(const ApplicationSpecs& specs)
    : m_specification(specs)
{
    PINE_CORE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    WindowSpecs window_specs;
    window_specs.title = m_specification.name;
    window_specs.width = m_specification.window_width;
    window_specs.height = m_specification.window_height;
    window_specs.fullscreen = m_specification.fullscreen;
    window_specs.vsync = m_specification.vsync;

    m_window = Window::create(window_specs);
    m_window->init();
    m_window->set_event_callback(PINE_BIND_EVENT_FN(Application::on_event));

    if (m_specification.start_maximized)
    {
        m_window->maximize();
    }
    else
    {
        m_window->center_window();
    }

    m_window->set_resizable(m_specification.resizable);
    m_window->set_vsync(m_specification.vsync);

    Renderer::Init();

    if (m_specification.enable_imgui)
    {
        m_imgui_layer = new ImGuiLayer();
        push_overlay(m_imgui_layer);
    }
}

Application::~Application()
{
    m_window->set_event_callback([](Event& event) {});
}

void Application::run()
{
    on_init();
    while (m_running)
    {
        // TODO: Temporary.
        auto time = glfwGetTime(); // Platform::GetTime
        m_timestep = time - m_last_frame_time;

        m_window->poll_events();

        // Update layers.
        if (!m_minimized)
        {
            m_last_frame_time = time;
            for (Layer* layer : m_layer_stack)
            {
                layer->on_update(m_timestep);
            }

            if (m_specification.enable_imgui)
            {
                render_imgui();
            }

            m_window->swap_buffers();
        }
    }
    on_shutdown();
}

void Application::close() { m_running = false; }

void Application::on_event(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowCloseEvent>(
        PINE_BIND_EVENT_FN(Application::on_window_close));
    dispatcher.dispatch<WindowResizeEvent>(
        PINE_BIND_EVENT_FN(Application::on_window_resize));
    dispatcher.dispatch<WindowIconifyEvent>(
        PINE_BIND_EVENT_FN(Application::on_window_iconify));

    for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();)
    {
        (*--it)->on_event(event);
        if (event.handled)
        {
            break;
        }
    }
}

void Application::push_layer(Layer* layer)
{
    m_layer_stack.push_layer(layer);
    layer->on_attach();
}

void Application::push_overlay(Layer* layer)
{
    m_layer_stack.push_overlay(layer);
    layer->on_attach();
}

void Application::pop_layer(Layer* layer)
{
    m_layer_stack.pop_layer(layer);
    layer->on_detach();
}

void Application::pop_overlay(Layer* layer)
{
    m_layer_stack.pop_overlay(layer);
    layer->on_detach();
}

void Application::render_imgui()
{
    m_imgui_layer->Begin();
    for (Layer* layer : m_layer_stack)
    {
        layer->on_imgui_render();
    }
    m_imgui_layer->End();
}

bool Application::on_window_close(WindowCloseEvent& event)
{
    m_running = false;
    return true;
}

bool Application::on_window_resize(WindowResizeEvent& event)
{
    Renderer::OnWindowResize(event.get_width(), event.get_height());
    return false;
}

bool Application::on_window_iconify(WindowIconifyEvent& event)
{
    m_minimized = event.is_minimized();
    return false;
}

}; // namespace pine
