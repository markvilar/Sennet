#include "Editor/EditorLayer.hpp"

#include <array>

namespace Sennet
{

InterfaceLayout CalculateMainMenuLayout(
    const uint32_t windowWidth, const uint32_t windowHeight)
{
    const Vec2 position = {0.0f, 0.0f};
    const Vec2 size = {0.0f, 0.0f};
    return InterfaceLayout(position, size);
}

InterfaceLayout CalculateViewportLayout(
    const uint32_t windowWidth, const uint32_t windowHeight)
{
    const Vec2 position = {0.2 * windowWidth, 0.0f};
    const Vec2 size = {0.6 * windowWidth, 0.8 * windowHeight};
    return InterfaceLayout(position, size);
}

InterfaceLayout CalculateLeftPanelLayout(
    const uint32_t windowWidth, const uint32_t windowHeight)
{
    const Vec2 position = {0.0f, 0.0f};
    const Vec2 size = {0.2 * windowWidth, windowHeight};
    return InterfaceLayout(position, size);
}

InterfaceLayout CalculateRightPanelLayout(
    const uint32_t windowWidth, const uint32_t windowHeight)
{
    const Vec2 position = {0.8 * windowWidth, 0.0f};
    const Vec2 size = {0.2 * windowWidth, windowHeight};
    return InterfaceLayout(position, size);
}

InterfaceLayout CalculateBottomPanelLayout(
    const uint32_t windowWidth, const uint32_t windowHeight)
{
    const Vec2 position = {0.2 * windowWidth, 0.8 * windowHeight};
    const Vec2 size = {0.6 * windowWidth, 0.2 * windowHeight};
    return InterfaceLayout(position, size);
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

    UI::SetDarkTheme(ImGui::GetStyle());
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(Timestep ts)
{
    // Resize framebuffer and update camera settings.
    auto specs = m_ViewportFramebuffer->GetSpecification();
    const auto windowSize = Sennet::Application::Get().GetWindow().GetSize();
    const auto viewport =
        CalculateViewportLayout(windowSize.first, windowSize.second);

    if (viewport.Size.x > 0.0f && viewport.Size.y > 0.0f
        && (specs.Width != viewport.Size.x || specs.Height != viewport.Size.y))
    {
        m_ViewportFramebuffer->Resize(viewport.Size.x, viewport.Size.y);
        m_CameraController.OnResize(viewport.Size.x, viewport.Size.y);
    }

    // Update camera.
    if (m_ViewportFocused)
        m_CameraController.OnUpdate(ts);

    // Render.
    Renderer2D::ResetStats();

    // Set background color.
    RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();

    // Set viewport background color.
    m_ViewportFramebuffer->Bind();
    RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();

    m_QuadRotation += ts * 50.0f;

    Renderer2D::BeginScene(m_CameraController.GetCamera());

    // Draw flat colored quads.
    Renderer2D::DrawRotatedQuad({0.0f, 0.0f},
        {0.8f, 0.8f},
        Radians(m_QuadRotation),
        {0.9f, 0.1f, 0.2f, 1.0f});
    Renderer2D::DrawQuad({2.0f, -2.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
    Renderer2D::DrawQuad({2.0f, 2.0f}, {0.5f, 0.75f}, m_QuadColor);

    // Draw grid of quads.
    constexpr auto ys = []() {
        std::array<float, 20> arr = {0.0f};
        auto value = -5.0f;
        auto increment = 0.5f;
        for (auto& entry : arr)
        {
            entry = value;
            value += increment;
        }
        return arr;
    }();

    constexpr auto xs = []() {
        std::array<float, 20> arr = {0.0f};
        auto value = -5.0f;
        auto increment = 0.5f;
        for (auto& entry : arr)
        {
            entry = value;
            value += increment;
        }
        return arr;
    }();

    for (auto y : ys)
    {
        for (auto x : xs)
        {
            Vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
            Renderer2D::DrawQuad({x, y, -0.1f}, {0.45f, 0.45f}, color);
        }
    }

    if (m_Texture)
    {
        Renderer2D::DrawQuad({-16.0f, 0.0f, -0.2f},
            {16.0f, 9.0f},
            m_Texture,
            1.0f,
            Vec4{1.0f, 1.0f, 1.0f, 1.0f});
    }

    Renderer2D::EndScene();
    m_ViewportFramebuffer->Unbind();
}

void EditorLayer::OnImGuiRender()
{
    const auto windowSize = Application::Get().GetWindow().GetSize();

    const auto mainMenuLayout =
        CalculateMainMenuLayout(windowSize.first, windowSize.second);
    const auto viewportLayout =
        CalculateViewportLayout(windowSize.first, windowSize.second);
    const auto leftInterfaceLayout =
        CalculateLeftPanelLayout(windowSize.first, windowSize.second);
    const auto rightInterfaceLayout =
        CalculateRightPanelLayout(windowSize.first, windowSize.second);
    const auto bottomInterfaceLayout =
        CalculateBottomPanelLayout(windowSize.first, windowSize.second);

    UI::AddMainMenuBar([]() {
        static bool showImGuiDemoWindow = false;
        static bool showImGuiMetrics = false;
        static bool showImGuiStackTool = false;
        static bool showFileSystemPopup = false;

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
            if (ImGui::MenuItem("Undo", "Ctrl+Z"))
            {
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y"))
            {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Directory"))
        {
            if (ImGui::MenuItem("Working directory"))
            {
                showFileSystemPopup = true;
            }
            if (ImGui::MenuItem("Asset directory"))
            {
                // TODO: Render asset directory panel.
            }
            ImGui::EndMenu();
        }

        if (showFileSystemPopup)
            ImGui::OpenPopup("WorkingDirectory");

        if (ImGui::BeginPopupModal("WorkingDirectory"))
        {
            static char workingDirectoryBuffer[256] = "";
            strcpy(workingDirectoryBuffer,
                Sennet::FileSystem::GetWorkingDirectory().c_str());
            ImGui::Text("Working directory: %s", workingDirectoryBuffer);
            if (ImGui::Button("Close"))
            {
                showFileSystemPopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginMenu("ImGui"))
        {
            ImGui::Checkbox("Show ImGui demo window", &showImGuiDemoWindow);
            ImGui::Checkbox("Show ImGui metrics", &showImGuiMetrics);
            ImGui::Checkbox("Show ImGui stack tool", &showImGuiStackTool);
            ImGui::EndMenu();
        }

        if (showImGuiDemoWindow)
            ImGui::ShowDemoWindow();
        if (showImGuiMetrics)
            ImGui::ShowMetricsWindow();
        if (showImGuiStackTool)
            ImGui::ShowStackToolWindow();
    });

    UI::AddViewport("Viewport",
        viewportLayout.Position,
        viewportLayout.Size,
        m_ViewportFramebuffer,
        [this] {
            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(
                !m_ViewportFocused || !m_ViewportHovered);
        });

    UI::AddWindow("Left",
        leftInterfaceLayout.Position,
        rightInterfaceLayout.Size,
        [this] {
            auto stats = Sennet::Renderer2D::GetStats();
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

            ImGui::ColorEdit4("Square Color", ValuePtr(m_QuadColor));

            static bool flipImage = false;
            static char imagePath[256] = "";
            ImGui::InputText("Image path", imagePath, IM_ARRAYSIZE(imagePath));
            if (ImGui::Button("Load image"))
            {
                if (Sennet::FileSystem::IsFile(imagePath))
                {
                    m_Texture =
                        Texture2D::Create(ReadImage(imagePath, flipImage));
                    SENNET_INFO("Loaded image: {}", imagePath);
                }
            }
            ImGui::SameLine();
            ImGui::Checkbox("Flip image", &flipImage);
        });

    UI::AddWindow("Right",
        rightInterfaceLayout.Position,
        rightInterfaceLayout.Size,
        [] {
            // TODO: Add functionality here.
            ImGui::TextUnformatted("This is a text field.");
        });

    UI::AddWindow("Bottom",
        bottomInterfaceLayout.Position,
        bottomInterfaceLayout.Size,
        [] {
            // TODO: Add functionality here.
            ImGui::TextUnformatted("This is a text field.");
        });
}

void EditorLayer::OnEvent(Event& e) { m_CameraController.OnEvent(e); }

} // namespace Sennet
