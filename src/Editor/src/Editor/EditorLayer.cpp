#include "Editor/EditorLayer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace Sennet
{

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
{
}

void EditorLayer::OnAttach()
{
    m_CheckerboardTexture =
        Texture2D::Create("../../../../assets/Textures/Checkerboard.png");

    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);

    Renderer2D::Init();
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(Timestep ts)
{
    SENNET_PROFILE_FUNCTION();

    // Resize framebuffer and camera.
    FramebufferSpecification spec = m_Framebuffer->GetSpecification();
    if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
        (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    {
        m_Framebuffer->Resize(
            (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    // Update camera.
    if (m_ViewportFocused)
        m_CameraController.OnUpdate(ts);

    // Render.
    Renderer2D::ResetStats();
    {
        SENNET_PROFILE_SCOPE("Renderer Prep");
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        SENNET_PROFILE_SCOPE("Renderer Draw");

        Renderer2D::BeginScene(m_CameraController.GetCamera());
        Renderer2D::DrawRotatedQuad({0.0f, 0.0f},
            {0.8f, 0.8f},
            glm::radians(rotation),
            {0.9f, 0.1f, 0.2f, 1.0f});
        Renderer2D::DrawQuad(
            {2.0f, -2.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
        Renderer2D::DrawQuad({2.0f, 2.0f}, {0.5f, 0.75f}, m_QuadColor);
        Renderer2D::DrawQuad({0.0f, 0.0f, -0.2f},
            {20.0f, 20.0f},
            m_CheckerboardTexture,
            4.0f,
            glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
        Renderer2D::DrawRotatedQuad({-2.0f, -2.0f, 0.0f},
            {1.0f, 1.0f},
            glm::radians(45.0f),
            m_CheckerboardTexture,
            1.0f,
            glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));

        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = {
                    (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
                Renderer2D::DrawQuad({x, y, -0.1f}, {0.45f, 0.45f}, color);
            }
        }

        Renderer2D::EndScene();
        m_Framebuffer->Unbind();
    }
}

void EditorLayer::OnImGuiRender()
{
    SENNET_PROFILE_FUNCTION();

    static bool dockspaceOpen = true;
    static bool optionFullscreenPersistant = true;
    bool optionFullscreen = optionFullscreenPersistant;

    /*
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (optionFullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (optionFullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
    }
    */

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
                Sennet::Application::Get().Close();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Settings window.
    ImGui::Begin("Settings");

    auto stats = Sennet::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_QuadColor));

    ImGui::End();

    // Viewport window.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(
        !m_ViewportFocused || !m_ViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID,
        ImVec2{viewportPanelSize.x, viewportPanelSize.y},
        ImVec2{0, 1},
        ImVec2{1, 0});
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void EditorLayer::OnEvent(Event& e) { m_CameraController.OnEvent(e); }

} // namespace Sennet
