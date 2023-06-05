#pragma once

#include <cstdint>
#include <memory>

#include "pine/renderer/renderer_api.hpp"
#include "pine/utils/math.hpp"

namespace pine
{

class RenderCommand
{
public:
    inline static void init() { s_renderer_api->init(); }

    inline static void set_viewport(const uint32_t x,
        const uint32_t y,
        const uint32_t width,
        const uint32_t height)
    {
        s_renderer_api->set_viewport(x, y, width, height);
    }

    inline static void set_clear_color(const Vec4& color)
    {
        s_renderer_api->set_clear_color(color);
    }

    inline static void clear() { s_renderer_api->clear(); }

    inline static void draw_indexed(const VertexArray& vertex_array,
        const uint32_t index_count = 0)
    {
        s_renderer_api->draw_indexed(vertex_array, index_count);
    }

private:
    static std::unique_ptr<RendererAPI> s_renderer_api;
};

} // namespace pine
