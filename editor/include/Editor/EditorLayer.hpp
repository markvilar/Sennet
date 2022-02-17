#pragma once

#include <memory>

#include "Pine/Pine.hpp"

namespace Pine
{

struct InterfaceLayout
{
    Vec2 Position = {0.0f, 0.0f};
    Vec2 Size = {0.0f, 0.0f};
};

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
    ShaderLibrary m_ShaderLibrary = {};

    Image m_Image;

    std::shared_ptr<Framebuffer> m_ViewportFramebuffer;
    std::shared_ptr<Texture2D> m_Texture;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    Renderer2D::RendererData m_RendererData2D = {};

    Vec4 m_QuadColor = {0.2f, 0.3f, 0.8f, 1.0f};
    float m_QuadRotation = 0.0f;

    // TODO: Debug, remove.
    std::array<Vec3, 5> m_QuadTranslations{};
    std::array<Vec4, 5> m_QuadColors{};
};

} // namespace Pine
