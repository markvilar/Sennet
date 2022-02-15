#include "Pine/Renderer/Renderer.hpp"

#include "Pine/Pch.hpp"
#include "Pine/Platform/OpenGL/OpenGLShader.hpp"
#include "Pine/Renderer/RenderCommand.hpp"
#include "Pine/Renderer/Renderer2D.hpp"

namespace Pine
{

std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData =
    std::make_unique<Renderer::SceneData>();

struct Data
{
    std::shared_ptr<ShaderLibrary> Library;
    std::shared_ptr<Texture2D> BlackTexture;
    std::shared_ptr<Texture2D> WhiteTexture;
};

static Data* s_Data = nullptr;

void Renderer::Init()
{
    s_Data = new Data();
    s_Data->Library = std::make_unique<ShaderLibrary>();

    Renderer::GetShaderLibrary()->Load("resources/shaders/Renderer2D.glsl");

    constexpr uint32_t width = 1;
    constexpr uint32_t height = 1;
    constexpr std::array<uint8_t, 4> whiteColor = {255, 255, 255, 255};
    constexpr std::array<uint8_t, 4> blackColor = {0, 0, 0, 255};

    s_Data->WhiteTexture = Texture2D::Create(
        Image(whiteColor.data(), width, height, ImageFormat::RGBA));
    s_Data->BlackTexture = Texture2D::Create(
        Image(blackColor.data(), width, height, ImageFormat::RGBA));

    RenderCommand::Init();
    Renderer2D::Init();
}

void Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
{
    RenderCommand::SetViewport(0, 0, width, height);
}

std::shared_ptr<ShaderLibrary>& Renderer::GetShaderLibrary()
{
    PINE_CORE_ASSERT(s_Data != nullptr, "Renderer un-initialized.");
    return s_Data->Library;
}

std::shared_ptr<Texture2D>& Renderer::GetBlackTexture()
{
    PINE_CORE_ASSERT(s_Data != nullptr, "Renderer un-initialized.");
    return s_Data->BlackTexture;
}

std::shared_ptr<Texture2D>& Renderer::GetWhiteTexture()
{
    PINE_CORE_ASSERT(s_Data != nullptr, "Renderer un-initialized.");
    return s_Data->WhiteTexture;
}

void Renderer::BeginScene(const OrthographicCamera& camera)
{
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(
    const Shader& shader, const VertexArray& vertexArray, const Mat4& transform)
{
    shader.Bind();
    shader.SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader.SetMat4("u_Transform", transform);

    vertexArray.Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

} // namespace Pine
