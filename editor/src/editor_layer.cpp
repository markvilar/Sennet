#include "editor/editor_layer.hpp"

#include <array>

namespace pine
{

EditorLayer::EditorLayer() : Layer("EditorLayer"), m_camera_controller(1.0f) {}

void EditorLayer::on_attach()
{
    update_interface_layout();

    auto& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("resources/fonts/OpenSans-Regular.ttf",
        15.0f,
        nullptr,
        io.Fonts->GetGlyphRangesCyrillic());

    if (!m_shader_library.load_shader("resources/shaders/quad_shader.glsl"))
    {
        PINE_ERROR("Failed to load shader.");
    }

    FramebufferSpecification specs;
    specs.Width = static_cast<uint32_t>(m_interface_layouts["Viewport"].Size.x);
    specs.Height =
        static_cast<uint32_t>(m_interface_layouts["Viewport"].Size.y);
    m_viewport_framebuffer = Framebuffer::create(specs);

    m_camera_controller.on_resize(m_interface_layouts["Viewport"].Size.x,
        m_interface_layouts["Viewport"].Size.y);

    m_quad_render_data = QuadRenderer::init();

    ui::set_dark_theme(ImGui::GetStyle());

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
    update_interface_layout();
    const auto& specs = m_viewport_framebuffer->get_specification();
    const auto viewport = m_interface_layouts["Viewport"];

    if (viewport.Size.x > 0.0f && viewport.Size.y > 0.0f
        && (static_cast<float>(specs.Width) != viewport.Size.x
            || static_cast<float>(specs.Height) != viewport.Size.y))
    {
        m_viewport_framebuffer->resize(static_cast<uint32_t>(viewport.Size.x),
            static_cast<uint32_t>(viewport.Size.y));
        m_camera_controller.on_resize(viewport.Size.x, viewport.Size.y);
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

    QuadRenderer::begin_scene(m_quad_render_data,
        m_camera_controller.get_camera());

    QuadRenderer::draw_rotated_quad(m_quad_render_data,
        {0.0f, 0.0f},
        {0.8f, 0.8f},
        Radians(m_quad_rotation),
        {0.9f, 0.1f, 0.2f, 1.0f});
    QuadRenderer::draw_quad(m_quad_render_data,
        {2.0f, -2.0f},
        {0.8f, 0.8f},
        {0.8f, 0.2f, 0.3f, 1.0f});
    QuadRenderer::draw_quad(m_quad_render_data,
        {2.0f, 2.0f},
        {0.5f, 0.75f},
        m_quad_color);

    if (m_texture)
    {
        QuadRenderer::draw_quad(m_quad_render_data,
            {-16.0f, 0.0f, -0.2f},
            {16.0f, 9.0f},
            m_texture,
            1.0f,
            Vec4{1.0f, 1.0f, 1.0f, 1.0f});
    }

    QuadRenderer::end_scene(m_quad_render_data);
    m_viewport_framebuffer->unbind();

    update_server(m_server,
        [this](const std::vector<uint8_t>& message) -> void
        { m_server_history.push_back(message); });
}

void EditorLayer::on_imgui_render()
{
    ui::main_menu_bar(
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
                    pine::filesystem::get_working_directory().c_str());
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

    ui::render_viewport("Viewport",
        m_interface_layouts["Viewport"].Position,
        m_interface_layouts["Viewport"].Size,
        *m_viewport_framebuffer.get(),
        [this]
        {
            m_viewport_focused = ImGui::IsWindowFocused();
            m_viewport_hovered = ImGui::IsWindowHovered();
            Application::get().get_imgui_layer()->block_events(
                !m_viewport_focused || !m_viewport_hovered);
        });

    ui::render_window("LeftPanel",
        m_interface_layouts["LeftPanel"].Position,
        m_interface_layouts["LeftPanel"].Size,
        [this]
        {
            auto& stats = m_quad_render_data.statistics;
            ImGui::Text("QuadRenderer Stats:");
            ImGui::Text("Draw Calls: %d", stats.draw_calls);
            ImGui::Text("Quads: %d", stats.quad_count);
            ImGui::Text("Vertices: %d", stats.get_total_vertex_count());
            ImGui::Text("Indices: %d", stats.get_total_index_count());

            ImGui::ColorEdit4("Square Color", ValuePtr(m_quad_color));

            ui::empty_space(0.0f, 10.0f);
            ImGui::Separator();

            for (const auto& [name, shader] : m_shader_library.get_shader_map())
            {
                ImGui::Text("%s", name.c_str());
            }

            ui::empty_space(0.0f, 10.0f);
            ImGui::Separator();

            static bool flip_image = false;
            static char image_path[256] = "";
            static auto image_format = ImageFormat::BGRA;

            const std::array<std::pair<const char*, ImageFormat>, 6>
                image_format_options = {
                    std::make_pair("Gray", ImageFormat::GRAY),
                    std::make_pair("Gray-alpha", ImageFormat::GRAY_ALPHA),
                    std::make_pair("RGB", ImageFormat::RGB),
                    std::make_pair("BGR", ImageFormat::BGR),
                    std::make_pair("RGBA", ImageFormat::RGBA),
                    std::make_pair("BGRA", ImageFormat::BGRA) };

            ImGui::InputText("Image path",
                image_path,
                IM_ARRAYSIZE(image_path));

            ui::dropdown("Image format", &image_format, image_format_options);
            
            ImGui::Checkbox("Flip image", &flip_image);
            ImGui::SameLine();
            if (ImGui::Button("Load image"))
            {
                if (pine::filesystem::is_file(image_path))
                {
                    m_texture = Texture2D::Create(
                        read_image(image_path, image_format, flip_image));
                    PINE_INFO("Loaded image: {0}, {1}",
                        image_path,
                        image_format);
                }
            }

            ui::empty_space(0.0f, 20.0f);
            ImGui::Separator();

            static int8_t value_int8 = 0;
            static int16_t value_int16 = 0;
            static int32_t value_int32 = 0;
            static int64_t value_int64 = 0;
            static uint8_t value_uint8 = 0;
            static uint16_t value_uint16 = 0;
            static uint32_t value_uint32 = 0;
            static uint64_t value_uint64 = 0;
            static float value_float = 0.0;
            static double value_double = 0.0;

            ui::slider_scalar<int8_t>("Slider int8", &value_int8, -10, 10);
            ui::slider_scalar<int16_t>("Slider int16", &value_int16, -10, 10);
            ui::slider_scalar<int32_t>("Slider int32", &value_int32, -10, 10);
            ui::slider_scalar<int64_t>("Slider int64", &value_int64, -10, 10);
            ui::slider_scalar<uint8_t>("Slider uint8", &value_uint8, 0, 10);
            ui::slider_scalar<uint16_t>("Slider uint16", &value_uint16, 0, 10);
            ui::slider_scalar<uint32_t>("Slider uint32", &value_uint32, 0, 10);
            ui::slider_scalar<uint64_t>("Slider uint64", &value_uint64, 0, 10);
            ui::slider_scalar<float>("Slider float", &value_float, -1.0f, 1.0f);
            ui::slider_scalar<double>("Slider double", &value_double, -1.0, 1.0);
        });

    ui::render_window("RightPanel",
        m_interface_layouts["RightPanel"].Position,
        m_interface_layouts["RightPanel"].Size,
        [this]()
        {
            static char address[256] = "";
            static uint16_t port = 0;
            ImGui::InputText("Address", address, IM_ARRAYSIZE(address));
            ImGui::InputInt("Port", reinterpret_cast<int*>(&port));
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

            static constexpr size_t message_size = 10 * 5;
            static char message_text[message_size] = "";
            ImGui::InputTextMultiline("Message",
                message_text,
                IM_ARRAYSIZE(message_text),
                ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 5),
                ImGuiInputTextFlags_AllowTabInput);

            if (ImGui::Button("Send to server"))
            {
                if (is_connected(m_client))
                {
                    const auto message = std::vector<uint8_t>(message_text,
                        message_text + sizeof(message_text));
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

            ui::empty_space(0.0f, 20.0f);

            ImGui::Text("Server messages:");
            for (const auto& message : m_server_history)
            {
                ImGui::Text("%s",
                    std::string(message.begin(), message.end()).c_str());
            }
        });

    ui::render_window("BottomPanel",
        m_interface_layouts["BottomPanel"].Position,
        m_interface_layouts["BottomPanel"].Size,
        []() {});
}

void EditorLayer::on_event(Event& event)
{
    m_camera_controller.on_event(event);
}

void EditorLayer::update_interface_layout()
{
    const auto& window_size = pine::Application::get().get_window().get_size();
    const auto window_width = static_cast<float>(window_size.first);
    const auto window_height = static_cast<float>(window_size.second);

    static constexpr auto menu_height = 20.0f;

    m_interface_layouts["Viewport"] =
        InterfaceLayout(Vec2(0.2f * window_width,
                            0.0f * window_height + menu_height),
            Vec2(0.6f * window_width, 0.8f * window_height));

    m_interface_layouts["LeftPanel"] =
        InterfaceLayout(Vec2(0.0f * window_width,
                            0.0f * window_height + menu_height),
            Vec2(0.2f * window_width, 1.0f * window_height - menu_height));

    m_interface_layouts["RightPanel"] =
        InterfaceLayout(Vec2(0.8f * window_width,
                            0.0f * window_height + menu_height),
            Vec2(0.2f * window_width, 1.0f * window_height - menu_height));

    m_interface_layouts["BottomPanel"] =
        InterfaceLayout(Vec2(0.2f * window_width,
                            0.8f * window_height + menu_height),
            Vec2(0.6f * window_width, 0.2f * window_height - menu_height));
}

} // namespace pine
