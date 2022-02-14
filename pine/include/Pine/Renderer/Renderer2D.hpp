#pragma once

#include <memory>

#include "Pine/Renderer/OrthographicCamera.hpp"
#include "Pine/Renderer/Texture.hpp"
#include "Pine/Utils/Math.hpp"

namespace Pine
{

class Renderer2D
{
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const OrthographicCamera& camera);
    static void EndScene();
    static void Flush();

    static void DrawQuad(
        const Vec2& position, const Vec2& size, const Vec4& color);
    static void DrawQuad(
        const Vec3& position, const Vec2& size, const Vec4& color);
    static void DrawQuad(const Vec2& position, const Vec2& size,
        const std::shared_ptr<Texture2D>& texture,
        const float tilingFactor = 1.0f, const Vec4& tintColor = Vec4(1.0f));
    static void DrawQuad(const Vec3& position, const Vec2& size,
        const std::shared_ptr<Texture2D>& texture,
        const float tilingFactor = 1.0f, const Vec4& tintColor = Vec4(1.0f));

    static void DrawRotatedQuad(const Vec2& position, const Vec2& size,
        const float rotation, const Vec4& color);
    static void DrawRotatedQuad(const Vec3& position, const Vec2& size,
        const float rotation, const Vec4& color);
    static void DrawRotatedQuad(const Vec2& position, const Vec2& size,
        const float rotation, const std::shared_ptr<Texture2D>& texture,
        const float tilingFactor = 1.0f, const Vec4& tintColor = Vec4(1.0f));
    static void DrawRotatedQuad(const Vec3& position, const Vec2& size,
        const float rotation, const std::shared_ptr<Texture2D>& texture,
        const float tilingFactor = 1.0f, const Vec4& tintColor = Vec4(1.0f));

    static void DrawQuad(const Mat4& transform, const Vec4& color);
    static void DrawQuad(const Mat4& transform,
        const std::shared_ptr<Texture2D>& texture,
        const float tilingFactor = 1.0f, const Vec4& tintColor = Vec4(1.0f));

    struct QuadData
    {
        static const uint32_t VerticesPerQuad = 4;
        static const uint32_t IndicesPerQuad = 6;
    };

    struct Statistics
    {
        uint32_t DrawCalls = 0;
        uint32_t QuadCount = 0;

        uint32_t GetTotalVertexCount()
        {
            return QuadCount * QuadData::VerticesPerQuad;
        }

        uint32_t GetTotalIndexCount()
        {
            return QuadCount * QuadData::IndicesPerQuad;
        }
    };

    static void ResetStats();
    static Statistics GetStats();

private:
    static void FlushAndReset();
};

} // namespace Pine
