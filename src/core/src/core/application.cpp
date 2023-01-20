#include "pine/core/application.hpp"

#include <GLFW/glfw3.h>

#include "pine/core/input.hpp"
#include "pine/core/log.hpp"
#include "pine/core/timestep.hpp"
#include "pine/pch.hpp"
#include "pine/renderer/renderer.hpp"

namespace pine
{

Application* Application::instance = nullptr;

Application::Application(const ApplicationSpecs& specs) : specification(specs)
{
    PINE_CORE_ASSERT(!instance, "Application already exists!");
    instance = this;

    WindowSpecs window_specs;
    window_specs.title = specification.name;
    window_specs.width = specification.window_width;
    window_specs.height = specification.window_height;
    window_specs.fullscreen = specification.fullscreen;
    window_specs.vsync = specification.vsync;

    window = Window::create(window_specs);
    window->init();
    window->set_event_callback(PINE_BIND_EVENT_FN(Application::on_event));

    if (specification.start_maximized)
    {
        window->maximize();
    }
    else
    {
        window->center_window();
    }

    window->set_resizable(specification.resizable);
    window->set_vsync(specification.vsync);

    Renderer::init();

    gui = gui::create_manager(window.get());
}

Application::~Application()
{
    window->set_event_callback([]([[maybe_unused]] Event& event) {});
}

void Application::run()
{
    on_init();
    while (running)
    {
        // TODO: Temporary.
        auto time = static_cast<float>(glfwGetTime()); // Platform::GetTime
        Timestep ts = time - last_frame_time;

        window->poll_events();

        // Update layers.
        if (!minimized)
        {
            last_frame_time = time;
            for (Layer* layer : layer_stack)
            {
                layer->on_update(ts);
            }

            if (specification.enable_gui)
            {
                render_gui();
            }

            window->swap_buffers();
        }
    }
    on_shutdown();
}

void Application::close() { running = false; }

void Application::on_event(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowCloseEvent>(
        [this](WindowCloseEvent& event) -> bool {
            return on_window_close(event);
        });
    dispatcher.dispatch<WindowResizeEvent>(
        [this](WindowResizeEvent& event) -> bool {
            return on_window_resize(event);
        });
    dispatcher.dispatch<WindowIconifyEvent>(
        [this](WindowIconifyEvent& event) -> bool {
            return on_window_iconify(event);
        });

    // Handle event in the GUI first.
    gui->on_event(event);

    // Propagate event down the layer stack.
    for (auto it = layer_stack.end(); it != layer_stack.begin();)
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
    layer_stack.push_layer(layer);
    layer->on_attach();
}

void Application::push_overlay(Layer* layer)
{
    layer_stack.push_overlay(layer);
    layer->on_attach();
}

void Application::pop_layer(Layer* layer)
{
    layer_stack.pop_layer(layer);
    layer->on_detach();
}

void Application::pop_overlay(Layer* layer)
{
    layer_stack.pop_overlay(layer);
    layer->on_detach();
}

void Application::render_gui()
{
    if (gui)
    {
        gui->begin_frame();
        for (Layer* layer : layer_stack)
        {
            layer->on_gui_render();
        }
        gui->end_frame();
    }
}

bool Application::on_window_close([[maybe_unused]] WindowCloseEvent& event)
{
    running = false;
    return true;
}

bool Application::on_window_resize(WindowResizeEvent& event)
{
    Renderer::on_window_resize(event.get_width(), event.get_height());
    return false;
}

bool Application::on_window_iconify(WindowIconifyEvent& event)
{
    minimized = event.is_minimized();
    return false;
}

} // namespace pine
