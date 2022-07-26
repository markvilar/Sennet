#pragma once

#include <memory>

#include "pine/renderer/camera.hpp"
#include "pine/renderer/shader.hpp"
#include "pine/renderer/texture.hpp"
#include "pine/utils/math.hpp"

namespace pine::QuadRenderer
{

static constexpr uint32_t MaxQuads = 20000;
static constexpr uint32_t MaxTextureSlots = 32;
static constexpr uint32_t VerticesPerQuad = 4;
static constexpr uint32_t IndicesPerQuad = 6;
static constexpr uint32_t MaxVertices = MaxQuads * VerticesPerQuad;
static constexpr uint32_t MaxIndices = MaxQuads * IndicesPerQuad;

static constexpr Vec4 QuadVertexPositions[VerticesPerQuad] = {
    {-0.5f, -0.5f, 0.0f, 1.0f},
    {0.5f, -0.5f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.0f, 1.0f},
};

static constexpr Vec2 QuadTextureCoords[VerticesPerQuad] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f},
};

struct QuadVertex
{
    Vec3 Position = {};
    Vec4 Color = {};
    Vec2 TextureCoord = {};
    uint32_t TextureIndex = {};
    float TilingFactor = {};
};

struct Statistics
{
    uint32_t DrawCalls = 0;
    uint32_t QuadCount = 0;

    uint32_t GetTotalVertexCount() { return QuadCount * VerticesPerQuad; }
    uint32_t GetTotalIndexCount() { return QuadCount * IndicesPerQuad; }
};

struct QuadRenderData
{
    std::unique_ptr<Shader> QuadShader = {};
    std::unique_ptr<VertexArray> QuadVertexArray = {};

    std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots{};
    std::array<QuadVertex, MaxVertices> QuadVertices{};

    uint32_t QuadVertexCount = 0;
    uint32_t QuadIndexCount = 0;
    uint32_t TextureSlotIndex = 0;

    Statistics Stats{};
};

QuadRenderData Init();
void Shutdown(QuadRenderData& data);

void BeginScene(QuadRenderData& data, const OrthographicCamera& camera);
void EndScene(QuadRenderData& data);

void Flush(QuadRenderData& data);
void FlushAndReset(QuadRenderData& data);

void DrawQuad(QuadRenderData& data, 
    const Vec2& position, 
    const Vec2& size,
    const Vec4& color);

void DrawQuad(QuadRenderData& data, 
    const Vec3& position, 
    const Vec2& size,
    const Vec4& color);

void DrawQuad(QuadRenderData& data, 
    const Vec2& position, 
    const Vec2& size,
    const std::shared_ptr<Texture2D>& texture, 
    const float tilingFactor = 1.0f,
    const Vec4& tintColor = Vec4(1.0f));

void DrawQuad(QuadRenderData& data, 
    const Vec3& position, 
    const Vec2& size,
    const std::shared_ptr<Texture2D>& texture, 
    const float tilingFactor = 1.0f,
    const Vec4& tintColor = Vec4(1.0f));

void DrawRotatedQuad(QuadRenderData& data, 
    const Vec2& position, 
    const Vec2& size,
    const float rotation, 
    const Vec4& color);

void DrawRotatedQuad(QuadRenderData& data, 
    const Vec3& position, 
    const Vec2& size,
    const float rotation, 
    const Vec4& color);

void DrawRotatedQuad(QuadRenderData& data, 
    const Vec2& position, 
    const Vec2& size,
    const float rotation, 
    const std::shared_ptr<Texture2D>& texture,
    const float tilingFactor = 1.0f, 
    const Vec4& tintColor = Vec4(1.0f));

void DrawRotatedQuad(QuadRenderData& data, 
    const Vec3& position, 
    const Vec2& size,
    const float rotation, 
    const std::shared_ptr<Texture2D>& texture,
    const float tilingFactor = 1.0f, 
    const Vec4& tintColor = Vec4(1.0f));

void DrawQuad(QuadRenderData& data, 
    const Mat4& transform, 
    const Vec4& color);

void DrawQuad(QuadRenderData& data, 
    const Mat4& transform,
    const std::shared_ptr<Texture2D>& texture, 
    const float tilingFactor = 1.0f,
    const Vec4& tintColor = Vec4(1.0f));

} // namespace pine::renderer2d
