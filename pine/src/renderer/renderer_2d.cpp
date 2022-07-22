#include "pine/renderer/renderer_2d.hpp"

#include "pine/renderer/buffer.hpp"
#include "pine/renderer/render_command.hpp"
#include "pine/renderer/renderer.hpp"
#include "pine/pch.hpp"

namespace pine::Renderer2D
{

RendererData Init()
{
    RendererData data;

    auto vertex_buffer = VertexBuffer::create(MaxVertices * sizeof(QuadVertex));
    vertex_buffer->set_layout({
        { "a_Position",     ShaderDataType::Float3 },
        { "a_Color",        ShaderDataType::Float4 },
        { "a_TexCoord",     ShaderDataType::Float2 },
        { "a_TexIndex",     ShaderDataType::Uint   },
        { "a_TilingFactor", ShaderDataType::Float  },
    });

    data.QuadVertexArray = VertexArray::create();
    data.QuadVertexArray->set_vertex_buffer(std::move(vertex_buffer));

    constexpr auto quadIndices = []()
    {
        std::array<uint32_t, MaxIndices> indices{};
        uint32_t offset = 0;
        for (uint32_t i = 0; i < indices.size(); i += IndicesPerQuad)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += VerticesPerQuad;
        }
        return indices;
    }();

    data.QuadVertexArray->set_index_buffer(
        IndexBuffer::create(quadIndices.data(), quadIndices.size()));

    constexpr auto samplers = []()
    {
        std::array<int32_t, MaxTextureSlots> samplers = {};
        for (uint32_t i = 0; i < samplers.size(); i++)
            samplers[i] = i;
        return samplers;
    }();

    data.QuadShader = Shader::Create("resources/shaders/Renderer2D.glsl");
    data.QuadShader->Bind();
    data.QuadShader->SetIntArray("u_Textures",
        samplers.data(),
        samplers.size());

    constexpr uint32_t width = 1;
    constexpr uint32_t height = 1;
    constexpr std::array<uint8_t, 4> whiteColor = {255, 255, 255, 255};
    data.TextureSlots[data.TextureSlotIndex++] = Texture2D::Create(
        Image(whiteColor.data(), width, height, ImageFormat::RGBA));

    return data;
}

void Shutdown(RendererData& data)
{
    data.QuadVertexCount = 0;
    data.QuadIndexCount = 0;
    data.TextureSlotIndex = 1;
}

void BeginScene(RendererData& data, const OrthographicCamera& camera)
{
    data.QuadShader->Bind();
    data.QuadShader->SetMat4("u_ViewProjection",
        camera.GetViewProjectionMatrix());

    data.QuadVertexCount = 0;
    data.QuadIndexCount = 0;
    data.TextureSlotIndex = 1;

    data.Stats.DrawCalls = 0;
    data.Stats.QuadCount = 0;
}
void EndScene(RendererData& data)
{
    auto& vertex_buffer = data.QuadVertexArray->get_vertex_buffer();
    vertex_buffer.set_data(data.QuadVertices.data(),
        data.QuadVertexCount * sizeof(QuadVertex));
    Flush(data);
}

void Flush(RendererData& data)
{
    for (uint32_t i = 0; i < data.TextureSlotIndex; i++)
        data.TextureSlots[i]->Bind(i);

    RenderCommand::DrawIndexed(*data.QuadVertexArray.get(),
        data.QuadIndexCount);

    data.Stats.DrawCalls++;
}

void FlushAndReset(RendererData& data)
{
    EndScene(data);
    data.QuadVertexCount = 0;
    data.QuadIndexCount = 0;
    data.TextureSlotIndex = 1;
}

void DrawQuad(RendererData& data, const Vec2& position, const Vec2& size,
    const Vec4& color)
{
    DrawQuad(data, {position.x, position.y, 0.0f}, size, color);
}

void DrawQuad(RendererData& data, const Vec3& position, const Vec2& size,
    const Vec4& color)
{
    Mat4 transform = Translate(Mat4(1.0f), position)
        * Scale(Mat4(1.0f), Vec3(size.x, size.y, 1.0f));

    DrawQuad(data, transform, color);
}

void DrawQuad(RendererData& data, const Vec2& position, const Vec2& size,
    const std::shared_ptr<Texture2D>& texture, const float tilingFactor,
    const Vec4& tintColor)
{
    DrawQuad(data,
        {position.x, position.y, 0.0f},
        size,
        texture,
        tilingFactor,
        tintColor);
}

void DrawQuad(RendererData& data, const Vec3& position, const Vec2& size,
    const std::shared_ptr<Texture2D>& texture, const float tilingFactor,
    const Vec4& tintColor)
{
    Mat4 transform = Translate(Mat4(1.0f), position)
        * Scale(Mat4(1.0f), Vec3(size.x, size.y, 1.0f));

    DrawQuad(data, transform, texture, tilingFactor, tintColor);
}

void DrawRotatedQuad(RendererData& data, const Vec2& position, const Vec2& size,
    const float rotation, const Vec4& color)
{
    DrawRotatedQuad(data,
        {position.x, position.y, 0.0f},
        size,
        rotation,
        color);
}

void DrawRotatedQuad(RendererData& data, const Vec3& position, const Vec2& size,
    const float rotation, const Vec4& color)
{
    Mat4 transform = Translate(Mat4(1.0f), position)
        * Rotate(Mat4(1.0f), rotation, Vec3(0.0f, 0.0f, 1.0f))
        * Scale(Mat4(1.0f), Vec3(size.x, size.y, 1.0f));

    DrawQuad(data, transform, color);
}

void DrawRotatedQuad(RendererData& data, const Vec2& position, const Vec2& size,
    const float rotation, const std::shared_ptr<Texture2D>& texture,
    const float tilingFactor, const Vec4& tintColor)
{
    DrawRotatedQuad(data,
        {position.x, position.y, 0.0f},
        size,
        rotation,
        texture,
        tilingFactor,
        tintColor);
}

void DrawRotatedQuad(RendererData& data, const Vec3& position, const Vec2& size,
    const float rotation, const std::shared_ptr<Texture2D>& texture,
    const float tilingFactor, const Vec4& tintColor)
{

    Mat4 transform = Translate(Mat4(1.0f), position)
        * Rotate(Mat4(1.0f), rotation, Vec3(0.0f, 0.0f, 1.0f))
        * Scale(Mat4(1.0f), Vec3(size.x, size.y, 1.0f));

    DrawQuad(data, transform, texture, tilingFactor, tintColor);
}

void UpdateQuadVertices(Renderer2D::RendererData& data, const Mat4& transform,
    const Vec4& color, const uint32_t textureIndex, const float tilingFactor)
{
    for (uint32_t i = 0; i < Renderer2D::VerticesPerQuad; i++)
    {
        data.QuadVertices[data.QuadVertexCount].Position =
            transform * Renderer2D::QuadVertexPositions[i];
        data.QuadVertices[data.QuadVertexCount].Color = color;
        data.QuadVertices[data.QuadVertexCount].TextureCoord =
            Renderer2D::QuadTextureCoords[i];
        data.QuadVertices[data.QuadVertexCount].TextureIndex = textureIndex;
        data.QuadVertices[data.QuadVertexCount].TilingFactor = tilingFactor;
        data.QuadVertexCount++;
    }

    data.QuadIndexCount += IndicesPerQuad;
    data.Stats.QuadCount++;
}

void DrawQuad(RendererData& data, const Mat4& transform, const Vec4& color)
{
    if (data.QuadIndexCount >= MaxIndices - IndicesPerQuad
        || data.QuadVertexCount >= MaxVertices - VerticesPerQuad)
    {
        FlushAndReset(data);
    }

    constexpr uint32_t textureIndex = 0; // White texture.
    constexpr float tilingFactor = 1.0f;

    UpdateQuadVertices(data, transform, color, textureIndex, tilingFactor);
}

void DrawQuad(RendererData& data, const Mat4& transform,
    const std::shared_ptr<Texture2D>& texture, const float tilingFactor,
    const Vec4& tintColor)
{
    if (data.QuadIndexCount >= MaxIndices - IndicesPerQuad
        || data.QuadVertexCount >= MaxVertices - VerticesPerQuad)
    {
        FlushAndReset(data);
    }

    uint32_t textureIndex = 0;
    for (uint32_t i = 1; i < data.TextureSlotIndex; i++)
    {
        if (*data.TextureSlots[i].get() == *texture.get())
        {
            textureIndex = i;
            break;
        }
    }

    if (textureIndex == 0)
    {
        textureIndex = data.TextureSlotIndex;
        data.TextureSlots[data.TextureSlotIndex] = texture;
        data.TextureSlotIndex++;
    }

    UpdateQuadVertices(data, transform, tintColor, textureIndex, tilingFactor);
}

} // namespace pine::renderer2d
