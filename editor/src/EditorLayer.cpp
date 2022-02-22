#include "Editor/EditorLayer.hpp"

#include <array>

namespace Pine
{

EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1.0f) {}

void EditorLayer::OnAttach()
{
    UpdateInterfaceLayout();

    auto& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("resources/fonts/OpenSans-Regular.ttf",
        15.0f,
        nullptr,
        io.Fonts->GetGlyphRangesCyrillic());

    if (!m_ShaderLibrary.Load("resources/shaders/Renderer2D.glsl"))
    {
        PINE_ERROR("Failed to load shader.");
    }

    Framebuffer::Specification specs;
    specs.Width = m_InterfaceLayouts["Viewport"].Size.x;
    specs.Height = m_InterfaceLayouts["Viewport"].Size.y;
    m_ViewportFramebuffer = Framebuffer::Create(specs);

    m_CameraController.OnResize(
        static_cast<uint32_t>(m_InterfaceLayouts["Viewport"].Size.x),
        static_cast<uint32_t>(m_InterfaceLayouts["Viewport"].Size.y));

    m_RendererData2D = Renderer2D::Init();

    UI::SetDarkTheme(ImGui::GetStyle());

    m_Server.Start();
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(Timestep ts)
{
    m_Server.Update();

    UpdateInterfaceLayout();
    auto specs = m_ViewportFramebuffer->GetSpecification();
    const auto viewport = m_InterfaceLayouts["Viewport"];

    if (viewport.Size.x > 0.0f && viewport.Size.y > 0.0f
        && (specs.Width != viewport.Size.x || specs.Height != viewport.Size.y))
    {
        m_ViewportFramebuffer->Resize(viewport.Size.x, viewport.Size.y);
        m_CameraController.OnResize(viewport.Size.x, viewport.Size.y);
    }

    if (m_ViewportFocused)
        m_CameraController.OnUpdate(ts);

    m_QuadRotation += ts * 50.0f;

    RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();

    m_ViewportFramebuffer->Bind();
    RenderCommand::SetClearColor({0.05f, 0.05f, 0.05f, 1.0f});
    RenderCommand::Clear();

    Renderer2D::BeginScene(m_RendererData2D, m_CameraController.GetCamera());

    Renderer2D::DrawRotatedQuad(m_RendererData2D,
        {0.0f, 0.0f},
        {0.8f, 0.8f},
        Radians(m_QuadRotation),
        {0.9f, 0.1f, 0.2f, 1.0f});
    Renderer2D::DrawQuad(m_RendererData2D,
        {2.0f, -2.0f},
        {0.8f, 0.8f},
        {0.8f, 0.2f, 0.3f, 1.0f});
    Renderer2D::DrawQuad(m_RendererData2D,
        {2.0f, 2.0f},
        {0.5f, 0.75f},
        m_QuadColor);

    if (m_Texture)
    {
        Renderer2D::DrawQuad(m_RendererData2D,
            {-16.0f, 0.0f, -0.2f},
            {16.0f, 9.0f},
            m_Texture,
            1.0f,
            Vec4{1.0f, 1.0f, 1.0f, 1.0f});
    }

    Renderer2D::EndScene(m_RendererData2D);
    m_ViewportFramebuffer->Unbind();
}

void EditorLayer::OnImGuiRender()
{
    UI::AddMainMenuBar([]() {
        static bool showImGuiDemoWindow = false;
        static bool showImGuiMetrics = false;
        static bool showImGuiStackTool = false;
        static bool showImGuiStyleEditor = false;
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
            ImGui::Checkbox("Show ImGui style editor", &showImGuiStyleEditor);
            ImGui::EndMenu();
        }

        if (showImGuiDemoWindow)
            ImGui::ShowDemoWindow();
        if (showImGuiMetrics)
            ImGui::ShowMetricsWindow();
        if (showImGuiStackTool)
            ImGui::ShowStackToolWindow();
        if (showImGuiStyleEditor)
        {
            ImGui::Begin("Dear ImGui Style Editor", &showImGuiStyleEditor);
            ImGui::ShowStyleEditor();
            ImGui::End();
        }
    });

    UI::AddViewport("Viewport",
        m_InterfaceLayouts["Viewport"].Position,
        m_InterfaceLayouts["Viewport"].Size,
        *m_ViewportFramebuffer.get(),
        [this] {
            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(
                !m_ViewportFocused || !m_ViewportHovered);
        });

    UI::AddWindow("LeftPanel",
        m_InterfaceLayouts["LeftPanel"].Position,
        m_InterfaceLayouts["LeftPanel"].Size,
        [this] {
            auto& stats = m_RendererData2D.Stats;
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

            ImGui::ColorEdit4("Square Color", ValuePtr(m_QuadColor));

            UI::AddEmptySpace(0.0f, 10.0f);
            ImGui::Separator();

            for (const auto& [name, shader] : m_ShaderLibrary.GetShaders())
            {
                ImGui::Text("%s", name.c_str());
            }

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

    UI::AddWindow("RightPanel",
        m_InterfaceLayouts["RightPanel"].Position,
        m_InterfaceLayouts["RightPanel"].Size,
        [this]() {
            static char address[256] = "";
            static uint16_t port = 0;
            ImGui::InputText("Address", address, IM_ARRAYSIZE(address));
            ImGui::InputInt("Port", (int*)&port);
            ImGui::Text("Client connected: %d", m_Client.IsConnected());

            if (ImGui::Button("Connect"))
            {
                m_Client.Connect(std::string(address), port);
            }
            ImGui::SameLine();
            if (ImGui::Button("Disconnect"))
            {
                m_Client.Disconnect();
            }

            static constexpr size_t messageSize = 1024 * 16;
            static char messageText[messageSize] = "";
            ImGui::InputTextMultiline("Message", 
                messageText, 
                IM_ARRAYSIZE(messageText), 
                ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), 
                ImGuiInputTextFlags_AllowTabInput);

            if (ImGui::Button("Send to server"))
            {
                if (m_Client.IsConnected())
                {
                    Message message;
                    message.Body = std::vector<uint8_t>(messageText, 
                        messageText + messageSize);
                    message.Header.Size = message.Body.size();
                    m_Client.Send(message);
                }
            }


            ImGui::Separator();
        });

    UI::AddWindow("BottomPanel",
        m_InterfaceLayouts["BottomPanel"].Position,
        m_InterfaceLayouts["BottomPanel"].Size,
        []() {});
}

void EditorLayer::OnEvent(Event& e) { m_CameraController.OnEvent(e); }

void EditorLayer::UpdateInterfaceLayout()
{
    const auto& [windowWidth, windowHeight] =
        Pine::Application::Get().GetWindow().GetSize();

    static constexpr auto menuHeight = 20.0f;

    const auto& mainMenuLayout = m_InterfaceLayouts["MainMenu"];

    m_InterfaceLayouts["Viewport"] = InterfaceLayout(
        Vec2(0.2f * windowWidth, 0.0f * windowHeight + menuHeight),
        Vec2(0.6f * windowWidth, 0.8f * windowHeight));

    m_InterfaceLayouts["LeftPanel"] = InterfaceLayout(
        Vec2(0.0f * windowWidth, 0.0f * windowHeight + menuHeight),
        Vec2(0.2f * windowWidth, 1.0f * windowHeight - menuHeight));

    m_InterfaceLayouts["RightPanel"] = InterfaceLayout(
        Vec2(0.8f * windowWidth, 0.0f * windowHeight + menuHeight),
        Vec2(0.2f * windowWidth, 1.0f * windowHeight - menuHeight));

    m_InterfaceLayouts["BottomPanel"] = InterfaceLayout(
        Vec2(0.2f * windowWidth, 0.8f * windowHeight + menuHeight),
        Vec2(0.6f * windowWidth, 0.2f * windowHeight - menuHeight));
}

} // namespace Pine
