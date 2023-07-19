#include "pine/core/engine.hpp"

#include <GLFW/glfw3.h>

#include "pine/pch.hpp"
#include "pine/core/input.hpp"
#include "pine/core/log.hpp"
#include "pine/core/timestep.hpp"
#include "pine/defines/assert.hpp"
#include "pine/renderer/renderer.hpp"

namespace pine
{

Engine* Engine::instance = nullptr;

void Engine::init()
{
    PINE_CORE_VERIFY(!instance, "An Engine instance already exists.");
    instance = this;

    // Initalize log
    Log::init();

    WindowSpecs window_specs;
    window_specs.title = specification.name;
    window_specs.width = specification.window_width;
    window_specs.height = specification.window_height;
    window_specs.fullscreen = specification.fullscreen;
    window_specs.vsync = specification.vsync;

    // Create window
    window = create_window(window_specs);
    window->init();
    window->set_event_callback(
        [this](const Event& event)
        { 
            events.push_back(event);
        });
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

    // Create GUI
    gui = gui::create_manager(window);

    // TODO: Remove this
    Renderer::init();

    state.initialized = true;
}

void Engine::shutdown()
{
    // TODO: Shut down components
    window->set_event_callback([]([[maybe_unused]] const Event& event) {});
    state.initialized = false;
}

void Engine::update(const Timestep& timestep)
{
    // TODO: Update components
}

void Engine::on_event(const Event& event)
{
    // Move events
    dispatch_event<Moved<Mouse>>(event, 
        [this](const Moved<Mouse>& key){
        });

    dispatch_event<Moved<MouseWheel>>(event, 
        [this](const Moved<MouseWheel>& key){
        });

    // Mouse button events
    dispatch_event<Pressed<MouseButton>>(event, 
        [this](const Pressed<MouseButton>& key){
        });

    dispatch_event<Released<MouseButton>>(event, 
        [this](const Released<MouseButton>& key){
        });

    // Key events
    dispatch_event<Pressed<Key>>(event, 
        [this](const Pressed<Key>& key){
        });

    dispatch_event<Released<Key>>(event, 
        [this](const Released<Key>& key){
        });

    // Window events
    dispatch_event<WindowClosed>(event, 
        [this](const WindowClosed& event_data){
            on_window_close(event_data);
        });
    
    dispatch_event<WindowIconified>(event, 
        [this](const WindowIconified& event_data){
            on_window_iconify(event_data);
        });
    
    dispatch_event<WindowResized>(event, 
        [this](const WindowResized& event_data){
            on_window_resize(event_data);
        });
        
    // Time elapsed
    dispatch_event<TimeElapsed>(event, 
        [this](const TimeElapsed& event_data){
        });

    // Handle event in the GUI
    if (gui)
        gui->on_event(event);
}

void Engine::on_window_close([[maybe_unused]] const WindowClosed& event)
{
    stop();
}

void Engine::on_window_iconify(const WindowIconified& event)
{
    state.minimized = event.minimized;
}

void Engine::on_window_resize(const WindowResized& event)
{
    Renderer::on_window_resize(event.width, event.height);
}
    
float Engine::get_time() const
{
    return static_cast<float>(glfwGetTime()); // TODO: Platform::GetTime
}

} // namespace pine
