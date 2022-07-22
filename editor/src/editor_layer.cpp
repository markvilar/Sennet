#include "editor/editor_layer.hpp"

#include <array>

namespace pine
{

EditorLayer::EditorLayer() : Layer("EditorLayer"), m_camera_controller(1.0f) {}

void EditorLayer::on_attach()
{
    UpdateInterfaceLayout();

    auto& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("resources/fonts/OpenSans-Regular.ttf",
        15.0f,
        nullptr,
        io.Fonts->GetGlyphRangesCyrillic());

    if (!m_shader_library.Load("resources/shaders/Renderer2D.glsl"))
    {
        PINE_ERROR("Failed to load shader.");
    }

    FramebufferSpecification specs;
    specs.Width = m_interface_layouts["Viewport"].Size.x;
    specs.Height = m_interface_layouts["Viewport"].Size.y;
    m_viewport_framebuffer = Framebuffer::create(specs);

    m_camera_controller.OnResize(static_cast<uint32_t>(
                                    m_interface_layouts["Viewport"].Size.x),
        static_cast<uint32_t>(m_interface_layouts["Viewport"].Size.y));

    m_renderer_data_2d = Renderer2D::Init();

    ui::SetDarkTheme(ImGui::GetStyle());

    start_server(m_server,
        [](const ConnectionState& connection) -> bool
        {
            PINE_INFO("Editor server: New connection {0}",
                connection.socket.remote_endpoint());
            return true;
        });
}

void EditorLayer::on_detach() {}

void EditorLayer::on_update(Timestep ts)
{
    UpdateInterfaceLayout();
    auto specs = m_viewport_framebuffer->get_specification();
    const auto viewport = m_interface_layouts["Viewport"];

    if (viewport.Size.x > 0.0f && viewport.Size.y > 0.0f
        && (specs.Width != viewport.Size.x || specs.Height != viewport.Size.y))
    {
        m_viewport_framebuffer->resize(viewport.Size.x, viewport.Size.y);
        m_camera_controller.OnResize(viewport.Size.x, viewport.Size.y);
    }

    if (m_viewport_focused)
    {
        // FIXME: Update to include window handle.
        // m_camera_controller.OnUpdate(ts);
    }

    m_quad_rotation += ts * 50.0f;

    RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();

    m_viewport_framebuffer->bind();
    RenderCommand::SetClearColor({0.05f, 0.05f, 0.05f, 1.0f});
    RenderCommand::Clear();

    Renderer2D::BeginScene(m_renderer_data_2d, m_camera_controller.GetCamera());

    Renderer2D::DrawRotatedQuad(m_renderer_data_2d,
        {0.0f, 0.0f},
        {0.8f, 0.8f},
        Radians(m_quad_rotation),
        {0.9f, 0.1f, 0.2f, 1.0f});
    Renderer2D::DrawQuad(m_renderer_data_2d,
        {2.0f, -2.0f},
        {0.8f, 0.8f},
        {0.8f, 0.2f, 0.3f, 1.0f});
    Renderer2D::DrawQuad(m_renderer_data_2d,
        {2.0f, 2.0f},
        {0.5f, 0.75f},
        m_quad_color);

    if (m_texture)
    {
        Renderer2D::DrawQuad(m_renderer_data_2d,
            {-16.0f, 0.0f, -0.2f},
            {16.0f, 9.0f},
            m_texture,
            1.0f,
            Vec4{1.0f, 1.0f, 1.0f, 1.0f});
    }

    Renderer2D::EndScene(m_renderer_data_2d);
    m_viewport_framebuffer->unbind();

    update_server(m_server,
        [this](const std::vector<uint8_t>& message) -> void
        { m_server_history.push_back(message); });
}

void EditorLayer::on_imgui_render()
{
    ui::AddMainMenuBar(
        []()
        {
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
                    pine::Application::get().close();
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
                    pine::filesystem::GetWorkingDirectory().c_str());
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
                ImGui::Checkbox("Show ImGui style editor",
                    &showImGuiStyleEditor);
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

    ui::AddViewport("Viewport",
        m_interface_layouts["Viewport"].Position,
        m_interface_layouts["Viewport"].Size,
        *m_viewport_framebuffer.get(),
        [this]
        {
            m_viewport_focused = ImGui::IsWindowFocused();
            m_viewport_hovered = ImGui::IsWindowHovered();
            Application::get().get_imgui_layer()->BlockEvents(
                !m_viewport_focused || !m_viewport_hovered);
        });

    ui::AddWindow("LeftPanel",
        m_interface_layouts["LeftPanel"].Position,
        m_interface_layouts["LeftPanel"].Size,
        [this]
        {
            auto& stats = m_renderer_data_2d.Stats;
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

            ImGui::ColorEdit4("Square Color", ValuePtr(m_quad_color));

            ui::AddEmptySpace(0.0f, 10.0f);
            ImGui::Separator();

            for (const auto& [name, shader] : m_shader_library.GetShaders())
            {
                ImGui::Text("%s", name.c_str());
            }

            ui::AddEmptySpace(0.0f, 10.0f);
            ImGui::Separator();

            static bool flip_image = false;
            static char image_path[256] = "";
            static auto image_format = ImageFormat::BGRA;

            const std::array<std::pair<std::string, ImageFormat>, 6>
                image_format_options = { 
                    std::make_pair("Gray", ImageFormat::GRAY),
                    std::make_pair("Gray-alpha", ImageFormat::GRAY_ALPHA),
                    std::make_pair("RGB", ImageFormat::RGB),
                    std::make_pair("BGR", ImageFormat::BGR),
                    std::make_pair("RGBA", ImageFormat::RGBA),
                    std::make_pair("BGRA", ImageFormat::BGRA)
                };

            ImGui::InputText("Image path", image_path, IM_ARRAYSIZE(image_path));
            ui::AddCombo("Image format", &image_format, image_format_options);
            ImGui::Checkbox("Flip image", &flip_image);
            ImGui::SameLine();
            if (ImGui::Button("Load image"))
            {
                if (pine::filesystem::IsFile(image_path))
                {
                    m_texture = Texture2D::Create(
                        read_image(image_path, image_format, flip_image));
                    PINE_INFO("Loaded image: {0}, {1}", image_path, 
                        image_format);
                }
            }

            ui::AddEmptySpace(0.0f, 20.0f);
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

            ui::SliderScalar("Slider int8", &valueInt8, -10, 10);
            ui::SliderScalar("Slider int16", &valueInt16, -10, 10);
            ui::SliderScalar("Slider int32", &valueInt32, -10, 10);
            ui::SliderScalar("Slider int64", &valueInt64, -10, 10);
            ui::SliderScalar("Slider uint8", &valueUint8, 0, 10);
            ui::SliderScalar("Slider uint16", &valueUint16, 0, 10);
            ui::SliderScalar("Slider uint32", &valueUint32, 0, 10);
            ui::SliderScalar("Slider uint64", &valueUint64, 0, 10);
            ui::SliderScalar("Slider float", &valueFloat, -1.0, 1.0);
            ui::SliderScalar("Slider double", &valueDouble, -1.0, 1.0);
        });

    ui::AddWindow("RightPanel",
        m_interface_layouts["RightPanel"].Position,
        m_interface_layouts["RightPanel"].Size,
        [this]()
        {
            static char address[256] = "";
            static uint16_t port = 0;
            ImGui::InputText("Address", address, IM_ARRAYSIZE(address));
            ImGui::InputInt("Port", (int*)&port);
            ImGui::Text("Client connected: %d", is_connected(m_client));

            if (ImGui::Button("Connect"))
            {
                connect(m_client, std::string(address), port);
            }
            ImGui::SameLine();
            if (ImGui::Button("Disconnect"))
            {
                disconnect(m_client);
            }

            static constexpr size_t messageSize = 10 * 5;
            static char messageText[messageSize] = "";
            ImGui::InputTextMultiline("Message",
                messageText,
                IM_ARRAYSIZE(messageText),
                ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 5),
                ImGuiInputTextFlags_AllowTabInput);

            if (ImGui::Button("Send to server"))
            {
                if (is_connected(m_client))
                {
                    const auto message = std::vector<uint8_t>(messageText,
                        messageText + sizeof(messageText));
                    send(m_client, message.data(), message.size());
                }
            }

            ImGui::Separator();

            ImGui::Text("Server");

            for (const auto& connection : m_server.connections)
            {
                const auto endpoint = connection->socket.remote_endpoint();
                ImGui::Text("Connection: %s:%d",
                    endpoint.address().to_string().c_str(),
                    endpoint.port());
            }

            ui::AddEmptySpace(0.0f, 20.0f);

            ImGui::Text("Server messages:");
            for (const auto& message : m_server_history)
            {
                ImGui::Text("%s",
                    std::string(message.begin(), message.end()).c_str());
            }
        });

    ui::AddWindow("BottomPanel",
        m_interface_layouts["BottomPanel"].Position,
        m_interface_layouts["BottomPanel"].Size,
        []() {});
}

void EditorLayer::on_event(Event& event) 
{ 
    m_camera_controller.OnEvent(event); 
}

void EditorLayer::UpdateInterfaceLayout()
{
    const auto& [windowWidth, windowHeight] =
        pine::Application::get().get_window().get_size();

    static constexpr auto menuHeight = 20.0f;

    const auto& mainMenuLayout = m_interface_layouts["MainMenu"];

    m_interface_layouts["Viewport"] =
        InterfaceLayout(Vec2(0.2f * windowWidth,
                            0.0f * windowHeight + menuHeight),
            Vec2(0.6f * windowWidth, 0.8f * windowHeight));

    m_interface_layouts["LeftPanel"] =
        InterfaceLayout(Vec2(0.0f * windowWidth,
                            0.0f * windowHeight + menuHeight),
            Vec2(0.2f * windowWidth, 1.0f * windowHeight - menuHeight));

    m_interface_layouts["RightPanel"] =
        InterfaceLayout(Vec2(0.8f * windowWidth,
                            0.0f * windowHeight + menuHeight),
            Vec2(0.2f * windowWidth, 1.0f * windowHeight - menuHeight));

    m_interface_layouts["BottomPanel"] =
        InterfaceLayout(Vec2(0.2f * windowWidth,
                            0.8f * windowHeight + menuHeight),
            Vec2(0.6f * windowWidth, 0.2f * windowHeight - menuHeight));
}

} // namespace pine
