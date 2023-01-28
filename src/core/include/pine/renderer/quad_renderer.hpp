#pragma once

#include "pine/renderer/camera.hpp"
#include "pine/renderer/shader.hpp"
#include "pine/renderer/texture.hpp"
#include "pine/utils/math.hpp"

#include <memory>

namespace pine
{
struct QuadVertex
{
    Vec3 position = {};
    Vec4 color = {};
    Vec2 texture_coordinates = {};
    uint32_t texture_index = {};
    float tiling_factor = {};
};

struct QuadRenderCaps
{
    static constexpr uint32_t max_quads = 20000;
    static constexpr uint32_t max_texture_slots = 32;
    static constexpr uint32_t vertices_per_quad = 4;
    static constexpr uint32_t indices_per_quad = 6;
    static constexpr uint32_t max_vertices = max_quads * vertices_per_quad;
    static constexpr uint32_t max_indices = max_quads * indices_per_quad;

    static constexpr Vec4 quad_vertex_positions[vertices_per_quad] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, 0.5f, 0.0f, 1.0f},
        {-0.5f, 0.5f, 0.0f, 1.0f},
    };

    static constexpr Vec2 quad_texture_coordinates[vertices_per_quad] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
    };
};

struct QuadRenderStatistics
{
    uint32_t draw_calls = 0;
    uint32_t quad_count = 0;

    uint32_t get_total_vertex_count()
    {
        return quad_count * QuadRenderCaps::vertices_per_quad;
    }

    uint32_t get_total_index_count()
    {
        return quad_count * QuadRenderCaps::indices_per_quad;
    }
};

struct QuadRenderData
{
    std::unique_ptr<Shader> quad_shader = {};
    std::unique_ptr<VertexArray> quad_vertex_array = {};

    std::array<std::shared_ptr<Texture2D>, QuadRenderCaps::max_texture_slots>
        texture_slots{};
    std::array<QuadVertex, QuadRenderCaps::max_vertices> quad_vertices{};

    uint32_t quad_vertex_count = 0;
    uint32_t quad_index_count = 0;
    uint32_t texture_slot_index = 0;

    QuadRenderStatistics statistics{};
}; // QuadRenderData

namespace QuadRenderer
{
QuadRenderData init();
void shutdown(QuadRenderData& data);

void begin_scene(QuadRenderData& data, const OrthographicCamera& camera);
void end_scene(QuadRenderData& data);

void flush(QuadRenderData& data);
void flush_and_reset(QuadRenderData& data);

void draw_quad(QuadRenderData& data,
    const Vec2& position,
    const Vec2& size,
    const Vec4& color);
void draw_quad(QuadRenderData& data,
    const Vec3& position,
    const Vec2& size,
    const Vec4& color);
void draw_quad(QuadRenderData& data,
    const Vec2& position,
    const Vec2& size,
    const std::shared_ptr<Texture2D>& texture,
    const float tilingFactor = 1.0f,
    const Vec4& tintColor = Vec4(1.0f));
void draw_quad(QuadRenderData& data,
    const Vec3& position,
    const Vec2& size,
    const std::shared_ptr<Texture2D>& texture,
    const float tilingFactor = 1.0f,
    const Vec4& tintColor = Vec4(1.0f));

void draw_rotated_quad(QuadRenderData& data,
    const Vec2& position,
    const Vec2& size,
    const float rotation,
    const Vec4& color);

void draw_rotated_quad(QuadRenderData& data,
    const Vec3& position,
    const Vec2& size,
    const float rotation,
    const Vec4& color);

void draw_rotated_quad(QuadRenderData& data,
    const Vec2& position,
    const Vec2& size,
    const float rotation,
    const std::shared_ptr<Texture2D>& texture,
    const float tilingFactor = 1.0f,
    const Vec4& tintColor = Vec4(1.0f));

void draw_rotated_quad(QuadRenderData& data,
    const Vec3& position,
    const Vec2& size,
    const float rotation,
    const std::shared_ptr<Texture2D>& texture,
    const float tilingFactor = 1.0f,
    const Vec4& tintColor = Vec4(1.0f));

void draw_quad(QuadRenderData& data, const Mat4& transform, const Vec4& color);

void draw_quad(QuadRenderData& data,
    const Mat4& transform,
    const std::shared_ptr<Texture2D>& texture,
    const float tilingFactor = 1.0f,
    const Vec4& tintColor = Vec4(1.0f));
} // namespace QuadRenderer

} // namespace pine
