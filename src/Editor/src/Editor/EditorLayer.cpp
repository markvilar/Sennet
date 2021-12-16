#include "Editor/EditorLayer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace Sennet
{

const PanelLayout CalculateViewportLayout(
    const uint32_t windowWidth, const uint32_t windowHeight)
{
    const glm::vec2 position = {0.2 * windowWidth, 0.0f};
    const glm::vec2 size = {0.6 * windowWidth, 0.8 * windowHeight};
    return PanelLayout(position, size);
}

const PanelLayout CalculateLeftPanelLayout(
    const uint32_t windowWidth, const uint32_t windowHeight)
{
    const glm::vec2 position = {0.0f, 0.0f};
    const glm::vec2 size = {0.2 * windowWidth, windowHeight};
    return PanelLayout(position, size);
}

const PanelLayout CalculateRightPanelLayout(
    const uint32_t windowWidth, const uint32_t windowHeight)
{
    const glm::vec2 position = {0.8 * windowWidth, 0.0f};
    const glm::vec2 size = {0.2 * windowWidth, windowHeight};
    return PanelLayout(position, size);
}

const PanelLayout CalculateBottomPanelLayout(
    const uint32_t windowWidth, const uint32_t windowHeight)
{
    const glm::vec2 position = {0.2 * windowWidth, 0.8 * windowHeight};
    const glm::vec2 size = {0.6 * windowWidth, 0.2 * windowHeight};
    return PanelLayout(position, size);
}

EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1.0f) {}

void EditorLayer::OnAttach()
{
    const auto windowSize = Sennet::Application::Get().GetWindow().GetSize();
    const auto viewport =
        CalculateViewportLayout(windowSize.first, windowSize.second);

    Framebuffer::Specification specs;
    specs.Width = viewport.Size.x;
    specs.Height = viewport.Size.y;
    m_ViewportFramebuffer = Framebuffer::Create(specs);
    m_CameraController.OnResize(static_cast<uint32_t>(viewport.Size.x),
        static_cast<uint32_t>(viewport.Size.y));

    /*
    TODO: Fix texture API to allow for this syntax.
    m_CheckerboardTexture =
        Texture2D::Create("../../resources/textures/Checkerboard.png");
    */
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(Timestep ts)
{
    SENNET_PROFILE_FUNCTION();

    // Resize framebuffer and update camera settings.
    auto specs = m_ViewportFramebuffer->GetSpecification();
    const auto windowSize = Sennet::Application::Get().GetWindow().GetSize();
    const auto viewport =
        CalculateViewportLayout(windowSize.first, windowSize.second);

    if (viewport.Size.x > 0.0f && viewport.Size.y > 0.0f
        && (specs.Width != viewport.Size.x || specs.Height != viewport.Size.y))
    {
        m_ViewportFramebuffer->Resize((uint32_t)viewport.Size.x,
            (uint32_t)viewport.Size.y);
        m_CameraController.OnResize(viewport.Size.x, viewport.Size.y);
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

        // Draw flat colored quads.
        Renderer2D::DrawRotatedQuad({0.0f, 0.0f},
            {0.8f, 0.8f},
            glm::radians(m_QuadRotation),
            {0.9f, 0.1f, 0.2f, 1.0f});
        Renderer2D::DrawQuad({2.0f, -2.0f},
            {0.8f, 0.8f},
            {0.8f, 0.2f, 0.3f, 1.0f});
        Renderer2D::DrawQuad({2.0f, 2.0f}, {0.5f, 0.75f}, m_QuadColor);

        /*
        TODO: Fix texture API to allow for this syntax.
        Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, { 20.0f, 20.0f },
            m_CheckerboardTexture, 4.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
        */

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

    const auto windowSize = Sennet::Application::Get().GetWindow().GetSize();
    const auto viewportLayout =
        CalculateViewportLayout(windowSize.first, windowSize.second);
    const auto leftPanelLayout =
        CalculateLeftPanelLayout(windowSize.first, windowSize.second);
    const auto rightPanelLayout =
        CalculateRightPanelLayout(windowSize.first, windowSize.second);
    const auto bottomPanelLayout =
        CalculateBottomPanelLayout(windowSize.first, windowSize.second);

    const auto fullscreen =
        Sennet::Application::Get().GetSpecification().Fullscreen;
    const auto windowFlags = ConfigureImGui(fullscreen);

    RenderMainMenu();

    // Viewport window.
    RenderViewportPanel(viewportLayout);
    RenderLeftPanel(leftPanelLayout);
    RenderRightPanel(rightPanelLayout);
    RenderBottomPanel(bottomPanelLayout);
}

void EditorLayer::OnEvent(Event& e) { m_CameraController.OnEvent(e); }

ImGuiWindowFlags EditorLayer::ConfigureImGui(const bool fullscreen)
{
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
    if (fullscreen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus;
    }

    if (fullscreen)
    {
        ImGui::PopStyleVar(2);
    }
    return windowFlags;
}

void EditorLayer::RenderViewportPanel(const PanelLayout& layout)
{
    ImGui::SetNextWindowPos(ImVec2(layout.Position.x, layout.Position.y),
        ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(layout.Size.x, layout.Size.y),
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
        ImVec2{layout.Size.x, layout.Size.y},
        ImVec2{0, 1},
        ImVec2{1, 0});
    ImGui::End();
    ImGui::PopStyleVar();
}

void EditorLayer::RenderLeftPanel(const PanelLayout& layout)
{
    ImGui::SetNextWindowPos(ImVec2(layout.Position.x, layout.Position.y),
        ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(layout.Size.x, layout.Size.y),
        ImGuiCond_Always);

    ImGui::Begin("LeftPanel",
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

    static char imagePath[256] = "";
    ImGui::InputText("Image path", imagePath, IM_ARRAYSIZE(imagePath));
    if (ImGui::Button("Load image"))
    {
        if (Sennet::FileSystem::IsFile(imagePath))
        {
            auto image = ReadImage(imagePath);
            SENNET_INFO("Image: {0}, {1}, {2}, {3}",
                image.Width,
                image.Height,
                image.Channels,
                image.Format);
        }
    }
    ImGui::End();
}

void EditorLayer::RenderRightPanel(const PanelLayout& layout)
{
    ImGui::SetNextWindowPos(ImVec2(layout.Position.x, layout.Position.y),
        ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(layout.Size.x, layout.Size.y),
        ImGuiCond_Always);

    ImGui::Begin("RightPanel",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoScrollbar);
    ImGui::End();
}

void EditorLayer::RenderBottomPanel(const PanelLayout& layout)
{
    ImGui::SetNextWindowPos(ImVec2(layout.Position.x, layout.Position.y),
        ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(layout.Size.x, layout.Size.y),
        ImGuiCond_Always);

    ImGui::Begin("BottomPanel",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoScrollbar);
    ImGui::End();
}

void EditorLayer::RenderMainMenu()
{
    // ImGui static variables.
    static bool showImGuiDemoWindow = false;
    static bool showImGuiMetrics = false;
    static bool showImGuiStackTool = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
            }
            if (ImGui::MenuItem("Exit", "Ctrl+W"))
            {
                Sennet::Application::Get().Close();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Editable 1"))
            {
            }
            if (ImGui::MenuItem("Editable 2"))
            {
            }
            if (ImGui::MenuItem("Editable 3"))
            {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Directory"))
        {
            if (ImGui::MenuItem("Working directory"))
            {
                ImGui::OpenPopup("FileSystem");
                
            }
            if (ImGui::BeginPopup("FileSystem"))
            {
                static char workingDirectoryBuffer[256] = "";
                strcpy(workingDirectoryBuffer,
                    Sennet::FileSystem::GetWorkingDirectory().c_str());
                ImGui::Text("Working directory: %s",
                    workingDirectoryBuffer);
                ImGui::EndPopup();
            }
            if (ImGui::MenuItem("Asset directory"))
            {
                // TODO: Render asset directory panel.
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Interface"))
        {
            ImGui::Checkbox("Show ImGui demo window", &showImGuiDemoWindow);
            ImGui::Checkbox("Show ImGui metrics", &showImGuiMetrics);
            ImGui::Checkbox("Show ImGui stack tool", &showImGuiStackTool);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (showImGuiDemoWindow)
        ImGui::ShowDemoWindow();
    if (showImGuiMetrics)
        ImGui::ShowMetricsWindow();
    if (showImGuiStackTool)
        ImGui::ShowStackToolWindow();
}

void EditorLayer::RenderFileSystemPopup() {}

void EditorLayer::RenderAssetPopup() {}

} // namespace Sennet
