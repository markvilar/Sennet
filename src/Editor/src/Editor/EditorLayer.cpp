#include "Editor/EditorLayer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace Sennet
{

EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1.0f) {}

void EditorLayer::OnAttach()
{
    UpdatePanelLayout();

    Framebuffer::Specification specs;
    specs.Width = m_ViewportSize.x;
    specs.Height = m_ViewportSize.y;
    m_ViewportFramebuffer = Framebuffer::Create(specs);
    m_CameraController.OnResize(static_cast<uint32_t>(m_ViewportSize.x),
        static_cast<uint32_t>(m_ViewportSize.y));
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(Timestep ts)
{
    SENNET_PROFILE_FUNCTION();

    // Resize framebuffer and update camera settings.
    auto specs = m_ViewportFramebuffer->GetSpecification();
    if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f
        && (specs.Width != m_ViewportSize.x
            || specs.Height != m_ViewportSize.y))
    {
        m_ViewportFramebuffer->Resize((uint32_t)m_ViewportSize.x,
            (uint32_t)m_ViewportSize.y);
        m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    // Update camera.
    if (m_ViewportFocused)
        m_CameraController.OnUpdate(ts);

    // Render.
    Renderer2D::ResetStats();
    {
        SENNET_PROFILE_SCOPE("Renderer Prep");

        // Set background color.
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        // Set viewport background color.
        m_ViewportFramebuffer->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();
    }

    {
        m_QuadRotation += ts * 50.0f;

        SENNET_PROFILE_SCOPE("Renderer Draw");

        Renderer2D::BeginScene(m_CameraController.GetCamera());

        // Draw individual quads.
        Renderer2D::DrawRotatedQuad({0.0f, 0.0f},
            {0.8f, 0.8f},
            glm::radians(m_QuadRotation),
            {0.9f, 0.1f, 0.2f, 1.0f});
        Renderer2D::DrawQuad({2.0f, -2.0f},
            {0.8f, 0.8f},
            {0.8f, 0.2f, 0.3f, 1.0f});
        Renderer2D::DrawQuad({2.0f, 2.0f}, {0.5f, 0.75f}, m_QuadColor);

        // Draw grid of quads.
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = {(x + 5.0f) / 10.0f,
                    0.4f,
                    (y + 5.0f) / 10.0f,
                    0.7f};
                Renderer2D::DrawQuad({x, y, -0.1f}, {0.45f, 0.45f}, color);
            }
        }

        Renderer2D::EndScene();
        m_ViewportFramebuffer->Unbind();
    }
}

void EditorLayer::OnImGuiRender()
{
    SENNET_PROFILE_FUNCTION();

    constexpr bool showDemoWindow = true;
    constexpr bool optionFullscreen = true;

    // Update panel sizes and positions.
    UpdatePanelLayout();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    if (optionFullscreen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

        window_flags |= ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus;
    }

    if (optionFullscreen)
        ImGui::PopStyleVar(2);

    // Menu bar.
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open"))
            {
            }
            if (ImGui::MenuItem("Save"))
            {
            }
            if (ImGui::MenuItem("Save As"))
            {
            }
            if (ImGui::MenuItem("Exit"))
            {
                Sennet::Application::Get().Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Set working directory"))
            {
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Settings window.
    ImGui::SetNextWindowPos(
        ImVec2(m_SettingsPanelPosition.x, m_SettingsPanelPosition.y),
        ImGuiCond_Always);
    ImGui::SetNextWindowSize(
        ImVec2(m_SettingsPanelSize.x, m_SettingsPanelSize.y),
        ImGuiCond_Always);

    ImGui::Begin("Settings",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar
            | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus);

    auto stats = Sennet::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_QuadColor));

    ImGui::End();

    // Viewport window.
    ImGui::SetNextWindowPos(ImVec2(m_ViewportPosition.x, m_ViewportPosition.y),
        ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(m_ViewportSize.x, m_ViewportSize.y),
        ImGuiCond_Always);

    ImGui::Begin("Viewport",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoScrollbar);

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(
        !m_ViewportFocused || !m_ViewportHovered);

    auto textureID = m_ViewportFramebuffer->GetColorAttachmentRendererID();
    ImGui::Image(reinterpret_cast<void*>(textureID),
        ImVec2{m_ViewportSize.x, m_ViewportSize.y},
        ImVec2{0, 1},
        ImVec2{1, 0});
    ImGui::End();
    ImGui::PopStyleVar();

    // Viewport window.
    ImGui::SetNextWindowPos(
        ImVec2(m_TestPanelPosition.x, m_TestPanelPosition.y),
        ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(m_TestPanelSize.x, m_TestPanelSize.y),
        ImGuiCond_Always);

    ImGui::Begin("Test",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoScrollbar);
    ImGui::End();

    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow();
    }
}

void EditorLayer::OnEvent(Event& e) { m_CameraController.OnEvent(e); }

void EditorLayer::UpdatePanelLayout()
{
    auto windowSize = Sennet::Application::Get().GetWindow().GetSize();

    m_SettingsPanelSize = {0.2 * windowSize.first, 0.8 * windowSize.second};
    m_SettingsPanelPosition = {0.0f, 0.0f};

    m_ViewportSize = {0.6 * windowSize.first, 0.8 * windowSize.second};
    m_ViewportPosition = {0.2 * windowSize.first, 0.0f};

    m_TestPanelSize = {0.2 * windowSize.first, 0.8 * windowSize.second};
    m_TestPanelPosition = {0.8 * windowSize.first, 0.0f};
}

} // namespace Sennet
