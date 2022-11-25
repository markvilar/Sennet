#include "pine/renderer/renderer.hpp"

#include "pine/pch.hpp"

#include "pine/renderer/quad_renderer.hpp"
#include "pine/renderer/render_command.hpp"

#include "pine/platform/opengl/shader.hpp"

namespace pine
{

std::unique_ptr<Renderer::SceneData> Renderer::s_scene_data =
    std::make_unique<Renderer::SceneData>();

void Renderer::init() { RenderCommand::init(); }

void Renderer::on_window_resize(const uint32_t width, const uint32_t height)
{
    RenderCommand::set_viewport(0, 0, width, height);
}

void Renderer::begin_scene(const OrthographicCamera& camera)
{
    s_scene_data->view_projection_matrix =
        camera.calculate_view_projection_matrix();
}

void Renderer::end_scene() {}

void Renderer::submit(const Shader& shader, const VertexArray& vertex_array,
    const Mat4& transform)
{
    shader.bind();
    shader.set_mat4("u_ViewProjection", s_scene_data->view_projection_matrix);
    shader.set_mat4("u_Transform", transform);

    vertex_array.bind();
    RenderCommand::draw_indexed(vertex_array);
}

} // namespace pine
