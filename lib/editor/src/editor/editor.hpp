#pragma once

#include <array>
#include <memory>

#include <pine/pine.hpp>

namespace pine
{

class Editor
{
public:
    Editor();
    ~Editor() = default;

    void init();
    void update(const Timestep& ts);
    void shutdown();

    void on_gui_render();
    void on_event(const Event& event);

private:
    void update_camera_controller(const Timestep& ts);
    void update_viewport();

private:
    // Rendering
    OrthographicCameraController camera_controller{1.0f};
    ShaderLibrary shader_library{};
    std::shared_ptr<Framebuffer> viewport_framebuffer;
    std::shared_ptr<Texture2D> texture;
    QuadRenderData quad_render_data{};

    // Network
    ClientState client{};
    ServerState server{5000};
    std::vector<std::vector<uint8_t>> server_history{};

    // Graphical interface
    gui::Window viewport_window{"Viewport"};
    gui::Window gui_control_window{"GUI Controls"};
    gui::Window network_window{"Network"};
    gui::Window renderer_window{"Renderer"};
    gui::Window console_window{"Console"};
    gui::Window test_window{"Test"};

    // Misc
    Vec4 quad_color{0.2f, 0.3f, 0.8f, 1.0f};
    float quad_rotation{0.0f};

    std::array<UUID, 5> uuids{};
};

} // namespace pine
