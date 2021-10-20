#include "Sennet/Renderer/Renderer.hpp"
#include "Sennet/Pch.hpp"

#include "Sennet/Renderer/RenderCommand.hpp"
#include "Sennet/Renderer/Renderer2D.hpp"

#include "Sennet/Platform/OpenGL/OpenGLShader.hpp"

namespace Sennet
{

Scope<Renderer::SceneData> Renderer::s_SceneData =
    CreateScope<Renderer::SceneData>();

/*
struct Renderer::Data
{
    Ref<ShaderLibrary> Library;
    Ref<Texture2D> BlackTexture;
    Ref<Texture2D> WhiteTexture;
};

static Renderer::Data* s_Data = nullptr;
*/

void Renderer::Init()
{
    //s_Data = new Data();
    //s_Data->Library = Ref<ShaderLibrary>();

    RenderCommand::Init();
    Renderer2D::Init();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height)
{
    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(OrthographicCamera& camera)
{
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader>& shader,
    const Ref<VertexArray>& vertexArray,
    const glm::mat4& transform)
{
    shader->Bind();
    shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader->SetMat4("u_Transform", transform);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

} // namespace Sennet
