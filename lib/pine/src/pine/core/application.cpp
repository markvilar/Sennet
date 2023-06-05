#include "pine/core/application.hpp"

#include <GLFW/glfw3.h>

#include "pine/pch.hpp"
#include "pine/core/input.hpp"
#include "pine/core/log.hpp"
#include "pine/core/timestep.hpp"
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

    // TODO: Separate factory method
    window = create_window(window_specs);
    window->init();
    window->set_event_callback([this](const Event& event){ on_event(event); });

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
    window->set_event_callback([]([[maybe_unused]] const Event& event) {});
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

void Application::on_event(const Event& event)
{
    if (std::holds_alternative<std::monostate>(event))
    {
        PINE_CORE_INFO("Got empty event!");
    }
    else if (std::holds_alternative<Moved<Mouse>>(event))
    {
        const auto content = std::get<Moved<Mouse>>(event);
    }
    else if (std::holds_alternative<Moved<MouseWheel>>(event))
    {
        const auto content = std::get<Moved<MouseWheel>>(event);
    }
    else if (std::holds_alternative<Pressed<MouseButton>>(event))
    {
        const auto content = std::get<Pressed<MouseButton>>(event);
    }
    else if (std::holds_alternative<Released<MouseButton>>(event))
    {
        const auto content = std::get<Released<MouseButton>>(event);
    }
    else if (std::holds_alternative<Pressed<Key>>(event))
    {
        const auto content = std::get<Pressed<Key>>(event);
    }
    else if (std::holds_alternative<Released<Key>>(event))
    {
        const auto content = std::get<Released<Key>>(event);
    }
    else if (std::holds_alternative<WindowClosed>(event))
    {
        const auto content = std::get<WindowClosed>(event);
        on_window_close(content);
    }
    else if (std::holds_alternative<WindowIconified>(event))
    {
        const auto content = std::get<WindowIconified>(event);
        on_window_iconify(content);
    }
    else if (std::holds_alternative<WindowResized>(event))
    {
        const auto content = std::get<WindowResized>(event);
        on_window_resize(content);
    }
    else if (std::holds_alternative<TimeElapsed>(event))
    {
        const auto content = std::get<TimeElapsed>(event);
    }
    else
    {
        PINE_CORE_INFO("Got invalid event!");
    }

    // Handle event in the GUI first.
    if (gui)
        gui->on_event(event);

    // Propagate event down the layer stack.
    for (auto it = layer_stack.end(); it != layer_stack.begin();)
    {
        (*--it)->on_event(event);
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

void Application::on_window_close([[maybe_unused]] const WindowClosed& event)
{
    running = false;
}

void Application::on_window_iconify(const WindowIconified& event)
{
    minimized = event.minimized;
}

void Application::on_window_resize(const WindowResized& event)
{
    Renderer::on_window_resize(event.width, event.height);
}

} // namespace pine
