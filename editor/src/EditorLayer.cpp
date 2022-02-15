#include "Editor/EditorLayer.hpp"

#include <array>

namespace Pine
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
    const auto windowSize = Pine::Application::Get().GetWindow().GetSize();
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
    const auto windowSize = Pine::Application::Get().GetWindow().GetSize();
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
                Pine::Application::Get().Close();
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
            if (ImGui::MenuItem("Resource directory"))
            {
                // TODO: Render resources directory panel.
            }
            ImGui::EndMenu();
        }

        if (showFileSystemPopup)
            ImGui::OpenPopup("WorkingDirectory");

        if (ImGui::BeginPopupModal("WorkingDirectory"))
        {
            static char workingDirectoryBuffer[256] = "";
            strcpy(workingDirectoryBuffer,
                Pine::FileSystem::GetWorkingDirectory().c_str());
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
            auto stats = Pine::Renderer2D::GetStats();
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

            ImGui::ColorEdit4("Square Color", ValuePtr(m_QuadColor));

            UI::AddEmptySpace(0.0f, 10.0f);
            ImGui::Separator();

            static bool flipImage = false;
            static char imagePath[256] = "";
            static auto imageFormat = ImageFormat::BGRA;

            const std::array<std::pair<std::string, ImageFormat>, 6>
                imageFormatOptions = {std::make_pair("Gray", ImageFormat::GRAY),
                    std::make_pair("Gray-alpha", ImageFormat::GRAY_ALPHA),
                    std::make_pair("RGB", ImageFormat::RGB),
                    std::make_pair("BGR", ImageFormat::BGR),
                    std::make_pair("RGBA", ImageFormat::RGBA),
                    std::make_pair("BGRA", ImageFormat::BGRA)};

            ImGui::InputText("Image path", imagePath, IM_ARRAYSIZE(imagePath));
            UI::AddCombo("Image format", &imageFormat, imageFormatOptions);
            ImGui::Checkbox("Flip image", &flipImage);
            ImGui::SameLine();
            if (ImGui::Button("Load image"))
            {
                if (Pine::FileSystem::IsFile(imagePath))
                {
                    m_Texture = Texture2D::Create(
                        ReadImage(imagePath, imageFormat, flipImage));
                    PINE_INFO("Loaded image: {0}, {1}", imagePath, imageFormat);
                }
            }

            UI::AddEmptySpace(0.0f, 20.0f);
            ImGui::Separator();

            static int8_t valueInt8 = 0;
            static int16_t valueInt16 = 0;
            static int32_t valueInt32 = 0;
            static int64_t valueInt64 = 0;
            static uint8_t valueUint8 = 0;
            static uint16_t valueUint16 = 0;
            static uint32_t valueUint32 = 0;
            static uint64_t valueUint64 = 0;
            static float valueFloat = 0.0;
            static double valueDouble = 0.0;

            UI::SliderScalar("Slider int8", &valueInt8, -10, 10);
            UI::SliderScalar("Slider int16", &valueInt16, -10, 10);
            UI::SliderScalar("Slider int32", &valueInt32, -10, 10);
            UI::SliderScalar("Slider int64", &valueInt64, -10, 10);
            UI::SliderScalar("Slider uint8", &valueUint8, 0, 10);
            UI::SliderScalar("Slider uint16", &valueUint16, 0, 10);
            UI::SliderScalar("Slider uint32", &valueUint32, 0, 10);
            UI::SliderScalar("Slider uint64", &valueUint64, 0, 10);
            UI::SliderScalar("Slider float", &valueFloat, -1.0, 1.0);
            UI::SliderScalar("Slider double", &valueDouble, -1.0, 1.0);
        });

    UI::AddWindow("Right",
        rightInterfaceLayout.Position,
        rightInterfaceLayout.Size,
        []() {});

    UI::AddWindow("Bottom",
        bottomInterfaceLayout.Position,
        bottomInterfaceLayout.Size,
        []() {});
}

void EditorLayer::OnEvent(Event& e) { m_CameraController.OnEvent(e); }

} // namespace Pine
