#pragma once
#include "Sennet/Sennet.hpp"

namespace Sennet
{

class EditorLayer : public Layer
{
public:
    EditorLayer();
    ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Timestep ts) override;

    virtual void OnImGuiRender() override;

    virtual void OnEvent(Event& e) override;

private:
    OrthographicCameraController m_CameraController;

    Ref<Framebuffer> m_Framebuffer;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    glm::vec2 m_ViewportSize = {0.0f, 0.0f};
    glm::vec4 m_QuadColor = {0.2f, 0.3f, 0.8f, 1.0f};
};

} // namespace Sennet
