#include "pine/renderer/quad_renderer.hpp"

#include "pine/pch.hpp"
#include "pine/renderer/buffer.hpp"
#include "pine/renderer/render_command.hpp"
#include "pine/renderer/renderer.hpp"

namespace pine {

constexpr bool is_indices_within_capabilities(const uint32_t index_count) {
    return index_count <
        (QuadRenderCaps::max_indices - QuadRenderCaps::indices_per_quad);
}

constexpr bool is_vertices_within_capabilities(const uint32_t vertex_count) {
    return vertex_count <
        (QuadRenderCaps::max_vertices - QuadRenderCaps::vertices_per_quad);
}

// Helper function used by quad draw functions.
void update_quad_vertices(QuadRenderData& data,
    const Mat4& transform,
    const Vec4& color,
    const uint32_t texture_index,
    const float tiling_factor) {
    for (uint32_t i = 0; i < QuadRenderCaps::vertices_per_quad; i++) {
        data.quad_vertices[data.quad_vertex_count].position =
            transform * QuadRenderCaps::quad_vertex_positions[i];
        data.quad_vertices[data.quad_vertex_count].color = color;
        data.quad_vertices[data.quad_vertex_count].texture_coordinates =
            QuadRenderCaps::quad_texture_coordinates[i];
        data.quad_vertices[data.quad_vertex_count].texture_index =
            texture_index;
        data.quad_vertices[data.quad_vertex_count].tiling_factor =
            tiling_factor;
        data.quad_vertex_count++;
    }

    data.quad_index_count += QuadRenderCaps::indices_per_quad;
    data.statistics.quad_count++;
}

QuadRenderData QuadRenderer::init() {
    QuadRenderData data;

    auto vertex_buffer =
        VertexBuffer::create(QuadRenderCaps::max_vertices * sizeof(QuadVertex));
    vertex_buffer->set_layout({
        {"a_Position",     ShaderDataType::FLOAT3},
        {"a_Color",        ShaderDataType::FLOAT4},
        {"a_TexCoord",     ShaderDataType::FLOAT2},
        {"a_TexIndex",     ShaderDataType::UINT  },
        {"a_TilingFactor", ShaderDataType::FLOAT },
    });

    data.quad_vertex_array = VertexArray::create();
    data.quad_vertex_array->set_vertex_buffer(std::move(vertex_buffer));

    constexpr auto quad_indices = []() {
        std::array<uint32_t, QuadRenderCaps::max_indices> indices{};
        uint32_t offset = 0;
        for (uint32_t i = 0; i < indices.size();
             i += QuadRenderCaps::indices_per_quad) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += QuadRenderCaps::vertices_per_quad;
        }
        return indices;
    }();

    data.quad_vertex_array->set_index_buffer(
        IndexBuffer::create(quad_indices.data(), quad_indices.size()));

    static constexpr auto samplers = []() {
        std::array<int32_t, QuadRenderCaps::max_texture_slots> samplers = {};
        for (uint32_t i = 0; i < samplers.size(); i++)
            samplers[i] = static_cast<int>(i);
        return samplers;
    }();

    data.quad_shader = create_shader("resources/shaders/quad.vert",
        "resources/shaders/quad.frag");
    data.quad_shader->bind();
    data.quad_shader->set_int_array("u_Textures",
        samplers.data(),
        samplers.size());

    static constexpr uint32_t width = 1;
    static constexpr uint32_t height = 1;
    static constexpr std::array<uint8_t, 4> white_color = {255, 255, 255, 255};
    data.texture_slots[data.texture_slot_index++] = Texture2D::create(
        Image(white_color.data(), width, height, ImageFormat::RGBA));

    return data;
}

void QuadRenderer::shutdown(QuadRenderData& data) {
    data.quad_vertex_count = 0;
    data.quad_index_count = 0;
    data.texture_slot_index = 1;
}

void QuadRenderer::begin_scene(QuadRenderData& data,
    const OrthographicCamera& camera) {
    data.quad_shader->bind();
    data.quad_shader->set_mat4("u_ViewProjection",
        camera.calculate_view_projection_matrix());

    data.quad_vertex_count = 0;
    data.quad_index_count = 0;
    data.texture_slot_index = 1;

    data.statistics.draw_calls = 0;
    data.statistics.quad_count = 0;
}
void QuadRenderer::end_scene(QuadRenderData& data) {
    auto& vertex_buffer = data.quad_vertex_array->get_vertex_buffer();
    vertex_buffer.set_data(data.quad_vertices.data(),
        static_cast<uint32_t>(data.quad_vertex_count * sizeof(QuadVertex)));
    flush(data);
}

void QuadRenderer::flush(QuadRenderData& data) {
    for (uint32_t i = 0; i < data.texture_slot_index; i++)
        data.texture_slots[i]->bind(i);

    RenderCommand::draw_indexed(*data.quad_vertex_array.get(),
        data.quad_index_count);

    data.statistics.draw_calls++;
}

void QuadRenderer::flush_and_reset(QuadRenderData& data) {
    end_scene(data);
    data.quad_vertex_count = 0;
    data.quad_index_count = 0;
    data.texture_slot_index = 1;
}

void QuadRenderer::draw_quad(QuadRenderData& data,
    const Vec2& position,
    const Vec2& size,
    const Vec4& color) {
    draw_quad(data, {position.x, position.y, 0.0f}, size, color);
}

void QuadRenderer::draw_quad(QuadRenderData& data,
    const Vec3& position,
    const Vec2& size,
    const Vec4& color) {
    Mat4 transform = translate(Mat4(1.0f), position) *
        scale(Mat4(1.0f), Vec3(size.x, size.y, 1.0f));

    draw_quad(data, transform, color);
}

void QuadRenderer::draw_quad(QuadRenderData& data,
    const Vec2& position,
    const Vec2& size,
    const std::shared_ptr<Texture2D>& texture,
    const float tiling_factor,
    const Vec4& tint_color) {
    draw_quad(data,
        {position.x, position.y, 0.0f},
        size,
        texture,
        tiling_factor,
        tint_color);
}

void QuadRenderer::draw_quad(QuadRenderData& data,
    const Vec3& position,
    const Vec2& size,
    const std::shared_ptr<Texture2D>& texture,
    const float tiling_factor,
    const Vec4& tint_color) {
    Mat4 transform = translate(Mat4(1.0f), position) *
        scale(Mat4(1.0f), Vec3(size.x, size.y, 1.0f));

    draw_quad(data, transform, texture, tiling_factor, tint_color);
}

void QuadRenderer::draw_rotated_quad(QuadRenderData& data,
    const Vec2& position,
    const Vec2& size,
    const float rotation,
    const Vec4& color) {
    draw_rotated_quad(data,
        {position.x, position.y, 0.0f},
        size,
        rotation,
        color);
}

void QuadRenderer::draw_rotated_quad(QuadRenderData& data,
    const Vec3& position,
    const Vec2& size,
    const float rotation,
    const Vec4& color) {
    Mat4 transform = translate(Mat4(1.0f), position) *
        rotate(Mat4(1.0f), rotation, Vec3(0.0f, 0.0f, 1.0f)) *
        scale(Mat4(1.0f), Vec3(size.x, size.y, 1.0f));

    draw_quad(data, transform, color);
}

void QuadRenderer::draw_rotated_quad(QuadRenderData& data,
    const Vec2& position,
    const Vec2& size,
    const float rotation,
    const std::shared_ptr<Texture2D>& texture,
    const float tiling_factor,
    const Vec4& tint_color) {
    draw_rotated_quad(data,
        {position.x, position.y, 0.0f},
        size,
        rotation,
        texture,
        tiling_factor,
        tint_color);
}

void QuadRenderer::draw_rotated_quad(QuadRenderData& data,
    const Vec3& position,
    const Vec2& size,
    const float rotation,
    const std::shared_ptr<Texture2D>& texture,
    const float tiling_factor,
    const Vec4& tint_color) {
    Mat4 transform = translate(Mat4(1.0f), position) *
        rotate(Mat4(1.0f), rotation, Vec3(0.0f, 0.0f, 1.0f)) *
        scale(Mat4(1.0f), Vec3(size.x, size.y, 1.0f));

    draw_quad(data, transform, texture, tiling_factor, tint_color);
}

void QuadRenderer::draw_quad(QuadRenderData& data,
    const Mat4& transform,
    const Vec4& color) {
    if (!is_indices_within_capabilities(data.quad_index_count) ||
        !is_vertices_within_capabilities(data.quad_vertex_count)) {
        flush_and_reset(data);
    }

    static constexpr uint32_t texture_index = 0; // White texture.
    static constexpr float tiling_factor = 1.0f;

    update_quad_vertices(data, transform, color, texture_index, tiling_factor);
}

void QuadRenderer::draw_quad(QuadRenderData& data,
    const Mat4& transform,
    const std::shared_ptr<Texture2D>& texture,
    const float tiling_factor,
    const Vec4& tint_color) {
    if (!is_indices_within_capabilities(data.quad_index_count) ||
        !is_vertices_within_capabilities(data.quad_vertex_count)) {
        flush_and_reset(data);
    }

    uint32_t texture_index = 0;
    for (uint32_t i = 1; i < data.texture_slot_index; i++) {
        if (*data.texture_slots[i].get() == *texture.get()) {
            texture_index = i;
            break;
        }
    }

    if (texture_index == 0) {
        texture_index = data.texture_slot_index;
        data.texture_slots[data.texture_slot_index] = texture;
        data.texture_slot_index++;
    }

    update_quad_vertices(data,
        transform,
        tint_color,
        texture_index,
        tiling_factor);
}

} // namespace pine
