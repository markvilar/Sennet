#pragma once

#include <memory>

#include "pine/pine.hpp"

namespace pine
{

class EditorLayer : public Layer
{
public:
    EditorLayer();
    ~EditorLayer() = default;

    virtual void on_attach() override;
    virtual void on_detach() override;
    virtual void on_update(const Timestep& ts) override;

    virtual void on_gui_render() override;

    virtual void on_event(Event& event) override;

private:
    void update_camera_controller(const Timestep& ts);

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
    gui::PanelState viewport_panel{};

    // Misc
    Vec4 quad_color{0.2f, 0.3f, 0.8f, 1.0f};
    float quad_rotation{0.0f};
};

} // namespace pine
