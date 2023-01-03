#include "editor/editor_layer.hpp"

#include <array>

#include <implot.h>

namespace pine
{

EditorLayer::EditorLayer() 
    : Layer("EditorLayer"), camera_controller(1.0f, true) 
{
}

void EditorLayer::on_attach()
{
    auto& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("resources/fonts/OpenSans-Regular.ttf",
        15.0f,
        nullptr,
        io.Fonts->GetGlyphRangesCyrillic());

    if (!shader_library.load_shader("resources/shaders/quad_shader.glsl"))
    {
        PINE_ERROR("Failed to load shader.");
    }

    gui::set_dark_theme(ImGui::GetStyle());

    FramebufferSpecs specs;
    specs.width = 0;
    specs.height = 0;
    viewport_framebuffer = Framebuffer::create(specs);

    quad_render_data = QuadRenderer::init();

    server.set_connection_callback(
        [](const ConnectionState& connection) -> bool
        {
            PINE_INFO("Editor server: New connection {0}",
                connection.socket.remote_endpoint());
            return true;
        }
    );
    
    server.set_message_callback(
        [this](const std::vector<uint8_t>& message) -> void
        { 
            server_history.push_back(message); 
        }
    );

    start_server(server);
}

void EditorLayer::on_detach() {}

void EditorLayer::on_update(const Timestep& ts)
{
    if (viewport_panel.focused)
    {
        update_camera_controller(ts);
    }

    quad_rotation += ts * 50.0f;

    RenderCommand::set_clear_color({0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::clear();

    viewport_framebuffer->bind();
    RenderCommand::set_clear_color({0.05f, 0.05f, 0.05f, 1.0f});
    RenderCommand::clear();

    QuadRenderer::begin_scene(quad_render_data,
        camera_controller.get_camera());

    QuadRenderer::draw_rotated_quad(quad_render_data,
        {0.0f, 0.0f},
        {0.8f, 0.8f},
        radians(quad_rotation),
        {0.9f, 0.1f, 0.2f, 1.0f});
    QuadRenderer::draw_quad(quad_render_data,
        {2.0f, -2.0f},
        {0.8f, 0.8f},
        {0.8f, 0.2f, 0.3f, 1.0f});
    QuadRenderer::draw_quad(quad_render_data,
        {2.0f, 2.0f},
        {0.5f, 0.75f},
        quad_color);

    if (texture)
    {
        QuadRenderer::draw_quad(quad_render_data,
            {-16.0f, 0.0f, -0.2f},
            {16.0f, 9.0f},
            texture,
            1.0f,
            Vec4{1.0f, 1.0f, 1.0f, 1.0f});
    }

    QuadRenderer::end_scene(quad_render_data);
    viewport_framebuffer->unbind();

    update_server(server);
}

void EditorLayer::on_gui_render()
{
    gui::render_dockspace("editor_dockspace");
    gui::main_menu_bar(
        []()
        {
            static bool show_imgui_demo_window = false;
            static bool show_imgui_metrics = false;
            static bool show_imgui_stack_tool = false;
            static bool show_imgui_style_editor = false;
            static bool show_filesystem_popup = false;

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
                    show_filesystem_popup = true;
                }
                if (ImGui::MenuItem("Resource directory"))
                {
                    // TODO: Render resources directory panel.
                }
                ImGui::EndMenu();
            }

            if (show_filesystem_popup)
                ImGui::OpenPopup("WorkingDirectory");

            if (ImGui::BeginPopupModal("WorkingDirectory"))
            {
                static char working_directory_buffer[256] = "";
                strcpy(working_directory_buffer,
                    pine::filesystem::get_working_directory().c_str());
                ImGui::Text("Working directory: %s", working_directory_buffer);
                if (ImGui::Button("Close"))
                {
                    show_filesystem_popup = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            if (ImGui::BeginMenu("Gui"))
            {
                ImGui::Checkbox("Show demo window", &show_imgui_demo_window);
                ImGui::Checkbox("Show metrics", &show_imgui_metrics);
                ImGui::Checkbox("Show stack tool", &show_imgui_stack_tool);
                ImGui::Checkbox("Show style editor",
                    &show_imgui_style_editor);
                ImGui::EndMenu();
            }

            if (show_imgui_demo_window)
                ImGui::ShowDemoWindow();
            if (show_imgui_metrics)
                ImGui::ShowMetricsWindow();
            if (show_imgui_stack_tool)
                ImGui::ShowStackToolWindow();
            if (show_imgui_style_editor)
            {
                ImGui::Begin("Dear ImGui Style Editor", 
                    &show_imgui_style_editor);
                ImGui::ShowStyleEditor();
                ImGui::End();
            }
        });

    viewport_panel = gui::render_viewport("Viewport", 
        *viewport_framebuffer.get());

    const auto& specs = viewport_framebuffer->get_specification();
    if (viewport_panel.size.x > 0.0f && viewport_panel.size.y > 0.0f
        && (static_cast<float>(specs.width) != viewport_panel.size.x
            || static_cast<float>(specs.height) != viewport_panel.size.y))
    {
        viewport_framebuffer->resize(
            static_cast<uint32_t>(viewport_panel.size.x),
            static_cast<uint32_t>(viewport_panel.size.y));
        camera_controller.on_resize(
            viewport_panel.size.x, 
            viewport_panel.size.y);
    }

    Application::get().get_graphical_interface().block_events(
        !viewport_panel.focused || !viewport_panel.hovered);

    gui::render_window("Renderer",
        [this]
        {
            auto& stats = quad_render_data.statistics;
            ImGui::Text("QuadRenderer Stats:");
            ImGui::Text("Draw Calls: %d", stats.draw_calls);
            ImGui::Text("Quads: %d", stats.quad_count);
            ImGui::Text("Vertices: %d", stats.get_total_vertex_count());
            ImGui::Text("Indices: %d", stats.get_total_index_count());

            ImGui::ColorEdit4("Square Color", value_ptr(quad_color));

            gui::empty_space(0.0f, 10.0f);
            ImGui::Separator();

            for (const auto& [name, shader] : shader_library.get_shader_map())
            {
                ImGui::Text("%s", name.c_str());
            }

            gui::empty_space(0.0f, 10.0f);
            ImGui::Separator();

            static bool flip_image = false;
            static char image_path[256] = "";
            static auto image_format = ImageFormat::BGRA;

            const std::array<std::pair<const char*, ImageFormat>, 6>
                image_format_options = {std::make_pair("Gray",
                                            ImageFormat::GRAY),
                    std::make_pair("Gray-alpha", ImageFormat::GRAY_ALPHA),
                    std::make_pair("RGB", ImageFormat::RGB),
                    std::make_pair("BGR", ImageFormat::BGR),
                    std::make_pair("RGBA", ImageFormat::RGBA),
                    std::make_pair("BGRA", ImageFormat::BGRA)};

            ImGui::InputText("Image path",
                image_path,
                IM_ARRAYSIZE(image_path));

            gui::dropdown("Image format", &image_format, image_format_options);

            ImGui::Checkbox("Flip image", &flip_image);
            ImGui::SameLine();
            if (ImGui::Button("Load image"))
            {
                if (pine::filesystem::is_file(image_path))
                {
                    texture = Texture2D::create(
                        read_image(image_path, image_format, flip_image));
                    PINE_INFO("Loaded image: {0}, {1}",
                        image_path,
                        image_format);
                }
            }

            gui::empty_space(0.0f, 20.0f);
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

            gui::slider_scalar<int8_t>("Slider int8", &value_int8, -10, 10);
            gui::slider_scalar<int16_t>("Slider int16", &value_int16, -10, 10);
            gui::slider_scalar<int32_t>("Slider int32", &value_int32, -10, 10);
            gui::slider_scalar<int64_t>("Slider int64", &value_int64, -10, 10);
            gui::slider_scalar<uint8_t>("Slider uint8", &value_uint8, 0, 10);
            gui::slider_scalar<uint16_t>("Slider uint16", &value_uint16, 0, 10);
            gui::slider_scalar<uint32_t>("Slider uint32", &value_uint32, 0, 10);
            gui::slider_scalar<uint64_t>("Slider uint64", &value_uint64, 0, 10);
            gui::slider_scalar<float>("Slider float", &value_float, -1.0f, 1.0f);
            gui::slider_scalar<double>("Slider double",
                &value_double,
                -1.0,
                1.0);
        });

    gui::render_window("Network",
        [this]()
        {
            static char address[256] = "";
            static uint16_t port = 0;
            ImGui::InputText("Address", address, IM_ARRAYSIZE(address));
            ImGui::InputInt("Port", reinterpret_cast<int*>(&port));
            ImGui::Text("Client connected: %d", is_connected(client));

            if (ImGui::Button("Connect"))
            {
                connect(client, std::string(address), port);
            }
            ImGui::SameLine();
            if (ImGui::Button("Disconnect"))
            {
                disconnect(client);
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
                if (is_connected(client))
                {
                    const auto message = std::vector<uint8_t>(message_text,
                        message_text + sizeof(message_text));
                    send(client, message.data(), message.size());
                }
            }

            ImGui::Separator();

            ImGui::Text("Server");

            for (const auto& connection : server.connections)
            {
                const auto endpoint = connection->socket.remote_endpoint();
                ImGui::Text("Connection: %s:%d",
                    endpoint.address().to_string().c_str(),
                    endpoint.port());
            }

            gui::empty_space(0.0f, 20.0f);

            ImGui::Text("Server messages:");
            for (const auto& message : server_history)
            {
                ImGui::Text("%s",
                    std::string(message.begin(), message.end()).c_str());
            }
        });

    gui::render_window("Console", [](){});

    gui::render_window("ImPlot", 
        []() 
        {
            if (ImPlot::BeginPlot("My Plot")) 
            {
                ImPlot::EndPlot();
            }
        });

}

void EditorLayer::on_event(Event& event)
{
    camera_controller.on_event(event);
}

void EditorLayer::update_camera_controller(const Timestep& ts)
{
    const auto& window = Application::get().get_window();
    const auto input_handle = pine::InputHandle::create(window);
    
    if (input_handle->is_key_pressed(KeyCode::A))
        camera_controller.move_left(ts);
    if (input_handle->is_key_pressed(KeyCode::D))
        camera_controller.move_right(ts);
    if (input_handle->is_key_pressed(KeyCode::W))
        camera_controller.move_up(ts);
    if (input_handle->is_key_pressed(KeyCode::S))
        camera_controller.move_down(ts);
    if (input_handle->is_key_pressed(KeyCode::Q))
        camera_controller.rotate_counter_clockwise(ts);
    if (input_handle->is_key_pressed(KeyCode::E))
        camera_controller.rotate_clockwise(ts);
}

} // namespace pine
