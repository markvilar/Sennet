#pragma once

#include "Sennet/Sennet.hpp"

namespace Sennet
{

struct InterfaceLayout
{
    Vec2 Position;
    Vec2 Size;

public:
    InterfaceLayout(const Vec2& position, const Vec2& size)
        : Position(position), Size(size)
    {
    }

    ~InterfaceLayout() = default;
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

    Image m_Image;

    std::shared_ptr<Framebuffer> m_ViewportFramebuffer;
    std::shared_ptr<Texture2D> m_Texture;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    Vec4 m_QuadColor = {0.2f, 0.3f, 0.8f, 1.0f};

    float m_QuadRotation = 0.0f;
};

} // namespace Sennet
