#pragma once

#include <memory>

#include "pine/core/common.hpp"
#include "pine/renderer/buffer.hpp"
#include "pine/utils/math.hpp"

namespace pine
{

using RendererID = uint32_t;

class RendererAPI
{
public:
    enum class API
    {
        None = 0,
        OpenGL = 1,
    };

public:
    virtual ~RendererAPI() = default;
    virtual void init() = 0;

    virtual void set_viewport(const uint32_t x, const uint32_t y,
        const uint32_t width, const uint32_t height) = 0;
    virtual void set_clear_color(const Vec4& color) = 0;
    virtual void clear() = 0;

    virtual void draw_indexed(const VertexArray& vertex_array,
        const uint32_t index_count = 0) = 0;

    inline static API get_api() { return s_api; }
    static std::unique_ptr<RendererAPI> create();

private:
    static API s_api;
};

} // namespace pine
