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
    virtual void on_update(Timestep ts) override;

    virtual void on_imgui_render() override;

    virtual void on_event(Event& event) override;

private:
    OrthographicCameraController m_camera_controller{1.0f};
    ShaderLibrary m_shader_library{};

    ClientState m_client{};
    ServerState m_server{5000};

    std::vector<std::vector<uint8_t>> m_server_history{};

    std::shared_ptr<Framebuffer> m_viewport_framebuffer;
    std::shared_ptr<Texture2D> m_texture;

    bool m_viewport_focused{false};
    bool m_viewport_hovered{false};

    QuadRenderData m_quad_render_data{};

    Vec4 m_quad_color{0.2f, 0.3f, 0.8f, 1.0f};
    float m_quad_rotation{0.0f};

    ui::PanelState m_viewport_panel{};
};

} // namespace pine
