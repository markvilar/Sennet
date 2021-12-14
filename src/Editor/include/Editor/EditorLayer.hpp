#pragma once
#include "Sennet/Sennet.hpp"

namespace Sennet
{

struct PanelLayout
{
    glm::vec2 Position;
    glm::vec2 Size;

public:
    PanelLayout(const glm::vec2& position, const glm::vec2& size)
        : Position(position), Size(size)
    {
    }
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
    ImGuiWindowFlags ConfigureImGui(const bool fullscreen);

    // ImGui functions.
    void RenderViewportPanel(const PanelLayout& layout);
    void RenderLeftPanel(const PanelLayout& layout);
    void RenderRightPanel(const PanelLayout& layout);
    void RenderBottomPanel(const PanelLayout& layout);

    void RenderMainMenu();
    void RenderFileSystemPopup();
    void RenderAssetPopup();

private:
    OrthographicCameraController m_CameraController;

    Ref<Framebuffer> m_ViewportFramebuffer;
    Ref<Texture2D> m_CheckerboardTexture;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    glm::vec4 m_QuadColor = {0.2f, 0.3f, 0.8f, 1.0f};

    float m_QuadRotation = 0.0f;
};

} // namespace Sennet
