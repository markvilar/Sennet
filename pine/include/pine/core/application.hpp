#pragma once

#include <cstdint>
#include <string>

#include "pine/core/common.hpp"
#include "pine/core/layer_stack.hpp"
#include "pine/core/timestep.hpp"
#include "pine/core/window.hpp"
#include "pine/events/application_event.hpp"
#include "pine/events/event.hpp"
#include "pine/renderer/renderer.hpp"
#include "pine/ui/imgui_layer.hpp"

int main(int argc, char** argv);

namespace pine
{

class Application
{
public:
    struct Specification
    {
        std::string working_directory;
        std::string name = "pine App";
        uint32_t window_width = 1600;
        uint32_t window_height = 800;
        bool start_maximized = true;
        bool vsync = true;
        bool resizable = true;
        bool enable_imgui = true;
        bool fullscreen = false;
    };

public:
    Application(const Specification& specs);
    virtual ~Application();

    void run();
    void close();

    virtual void on_init() {}
    virtual void on_shutdown() {}
    virtual void on_update() {}

    void on_event(Event& e);

    void push_layer(Layer* layer);
    void push_overlay(Layer* overlay);
    void pop_layer(Layer* layer);
    void pop_overlay(Layer* overlay);

    void render_imgui();

    inline Window& get_window() { return *m_window; }

    inline ImGuiLayer* get_imgui_layer() { return m_imgui_layer; }

    static inline Application& get() { return *s_instance; }

    inline const Specification& get_specification() const
    {
        return m_specification;
    }

private:
    bool on_window_close(WindowCloseEvent& event);
    bool on_window_resize(WindowResizeEvent& event);
    bool on_window_iconify(WindowIconifyEvent& event);

private:
    std::unique_ptr<Window> m_window;
    Specification m_specification;

    ImGuiLayer* m_imgui_layer;
    LayerStack m_layer_stack;

    bool m_running = true;
    bool m_minimized = false;
    float m_last_frame_time = 0.0f;
    Timestep m_timestep;

    static Application* s_instance;
};

// To be defined in client.
std::unique_ptr<Application> create_application(int argc, char** argv);

} // namespace pine
