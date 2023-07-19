#pragma once

#include <concepts>
#include <cstdint>
#include <deque>
#include <memory>
#include <string>

#include "pine/core/event.hpp"
#include "pine/core/log.hpp"
#include "pine/core/timestep.hpp"
#include "pine/core/window.hpp"
#include "pine/gui/manager.hpp"
#include "pine/renderer/renderer.hpp"

namespace pine
{

struct EngineSpecs
{
    std::string name = "Pine Engine";
    uint32_t window_width = 1600;
    uint32_t window_height = 800;
    bool fullscreen = false;
    bool vsync = true;

    std::string working_directory;
    bool start_maximized = true;
    bool resizable = true;
    bool enable_gui = true;
};

struct EngineState
{
    bool initialized = false;
    bool running = false;
    bool minimized = false;
    float last_frame_time = 0.0f;
};

template <typename T>
concept Runnable = requires(T t)
{
    t.init();
    t.update( pine::Timestep() );
    t.shutdown();

    t.on_gui_render();
    t.on_event( pine::Event() );
};

class Engine
{
public:
    Engine(const EngineSpecs& specs = EngineSpecs{}) : specification(specs) {}
    ~Engine() = default;


    template <Runnable App>
    auto run(App& app)
    {
        // Initialize engine and application
        init();
        app.init();

        // Prepare
        state.running = true;

        while (state.running)
        {
            // TODO: Temporary.
            const auto time = get_time();
            const Timestep timestep = time - state.last_frame_time;
            
            // Handle events
            window->poll_events();
            while (!events.empty())
            {
                const auto event = events.front();
                events.pop_front();
                on_event(event);
                app.on_event(event);
            }

            // Update engine and application
            update(timestep);
            app.update(timestep);
            
            // Render GUI and swap buffers
            if (!state.minimized)
            {
                state.last_frame_time = time;
                if (specification.enable_gui && gui)
                {
                    gui->begin_frame();
                    app.on_gui_render();
                    gui->end_frame();
                }
                window->swap_buffers();
            }
        }

        // Shut down application and engine
        app.shutdown();
        shutdown();
    }

    inline std::shared_ptr<Window> get_window() const { return window; }
    inline gui::Manager& get_gui() const { return *gui; }

    inline EngineState get_state() const { return state; }
    inline void stop() { state.running = false; };

    static inline Engine& get() { return *instance; }

    inline const EngineSpecs& get_specification() const
    {
        return specification;
    }

private:
    void init();
    void shutdown();
    void update(const Timestep& timestep);

    void on_event(const Event& event);

    void on_window_close(const WindowClosed& event);
    void on_window_resize(const WindowResized& event);
    void on_window_iconify(const WindowIconified& event);

    // NOTE: Temporary
    float get_time() const;

private:
    EngineSpecs specification;
    EngineState state;
    std::deque<Event> events;

    // Shared resources
    std::shared_ptr<Window> window;

    // Owned resources
    std::unique_ptr<gui::Manager> gui;

    // Singleton
    static Engine* instance;
};

} // namespace pine
