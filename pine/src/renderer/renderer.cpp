#include "pine/renderer/renderer.hpp"

#include "pine/pch.hpp"

#include "pine/renderer/quad_renderer.hpp"
#include "pine/renderer/render_command.hpp"

#include "pine/platform/opengl/shader.hpp"

namespace pine
{

std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData =
    std::make_unique<Renderer::SceneData>();

void Renderer::Init() { RenderCommand::Init(); }

void Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
{
    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(const OrthographicCamera& camera)
{
    s_SceneData->ViewProjectionMatrix =
        camera.calculate_view_projection_matrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Shader& shader, const VertexArray& vertex_array,
    const Mat4& transform)
{
    shader.bind();
    shader.set_mat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader.set_mat4("u_Transform", transform);

    vertex_array.bind();
    RenderCommand::DrawIndexed(vertex_array);
}

} // namespace pine
