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

Application::Application(const ApplicationSpecs& specs) : m_specification(specs)
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

    Renderer::init();

    m_gui = GraphicalInterface::create(m_window.get());
}

Application::~Application()
{
    m_window->set_event_callback([]([[maybe_unused]] Event& event) {});
}

void Application::run()
{
    on_init();
    while (m_running)
    {
        // TODO: Temporary.
        auto time = static_cast<float>(glfwGetTime()); // Platform::GetTime
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

            if (m_specification.enable_gui)
            {
                render_gui();
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

    // Handle event in the GUI first.
    m_gui->on_event(event);

    for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();)
    {
        if (event.handled)
        {
            break;
        }
        else
        {
            (*--it)->on_event(event);
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

void Application::render_gui()
{
    if (m_gui)
    {
        m_gui->begin_frame();
        for (Layer* layer : m_layer_stack)
        {
            layer->on_gui_render();
        }
        m_gui->end_frame();
    }
}

bool Application::on_window_close([[maybe_unused]] WindowCloseEvent& event)
{
    m_running = false;
    return true;
}

bool Application::on_window_resize(WindowResizeEvent& event)
{
    Renderer::on_window_resize(event.get_width(), event.get_height());
    return false;
}

bool Application::on_window_iconify(WindowIconifyEvent& event)
{
    m_minimized = event.is_minimized();
    return false;
}

} // namespace pine
