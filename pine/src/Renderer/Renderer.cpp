#include "Pine/Renderer/Renderer.hpp"

#include "Pine/Pch.hpp"
#include "Pine/Platform/OpenGL/OpenGLShader.hpp"
#include "Pine/Renderer/RenderCommand.hpp"
#include "Pine/Renderer/Renderer2D.hpp"

namespace Pine
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
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Shader& shader, const VertexArray& vertexArray,
    const Mat4& transform)
{
    shader.Bind();
    shader.SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader.SetMat4("u_Transform", transform);

    vertexArray.Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

} // namespace Pine
