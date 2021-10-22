#include "Sennet/Renderer/Renderer.hpp"
#include "Sennet/Pch.hpp"

#include "Sennet/Renderer/RenderCommand.hpp"
#include "Sennet/Renderer/Renderer2D.hpp"

#include "Sennet/Platform/OpenGL/OpenGLShader.hpp"

namespace Sennet
{

Scope<Renderer::SceneData> Renderer::s_SceneData =
    CreateScope<Renderer::SceneData>();

struct Data
{
    Ref<ShaderLibrary> Library;
    Ref<Texture2D> BlackTexture;
    Ref<Texture2D> WhiteTexture;
};

static Data* s_Data = nullptr;

void Renderer::Init()
{
    s_Data = new Data();

    SENNET_CORE_INFO("Initializing renderer.");

    // Initialize shader library and load shaders.
    s_Data->Library = CreateRef<ShaderLibrary>();
    SENNET_CORE_ASSERT(s_Data->Library, "Uninitialized shader library.");
    Renderer::GetShaderLibrary()->Load(
        "../../resources/shaders/Renderer2D.glsl");

    // TODO: Implement corresponding texture methods.
    constexpr uint32_t whiteTextureData = 0xffffffff;
    s_Data->WhiteTexture =
        Texture2D::Create(1, 1, Texture::DataFormat::RGBA, &whiteTextureData);

    // TODO: Implement corresponding texture methods.
    constexpr uint32_t blackTextureData = 0xff000000;
    s_Data->BlackTexture =
        Texture2D::Create(1, 1, Texture::DataFormat::RGBA, &blackTextureData);

    RenderCommand::Init();
    Renderer2D::Init();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height)
{
    RenderCommand::SetViewport(0, 0, width, height);
}

Ref<ShaderLibrary> Renderer::GetShaderLibrary()
{
    SENNET_CORE_ASSERT(s_Data != nullptr, "Renderer un-initialized.");
    return s_Data->Library;
}

Ref<Texture2D> Renderer::GetBlackTexture()
{
    SENNET_CORE_ASSERT(s_Data != nullptr, "Renderer un-initialized.");
    return s_Data->BlackTexture;
}

Ref<Texture2D> Renderer::GetWhiteTexture()
{
    SENNET_CORE_ASSERT(s_Data != nullptr, "Renderer un-initialized.");
    return s_Data->WhiteTexture;
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
