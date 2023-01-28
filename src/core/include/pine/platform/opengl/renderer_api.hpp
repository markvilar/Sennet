#pragma once

#include <cstdint>
#include <memory>

#include "pine/renderer/renderer_api.hpp"

namespace pine
{
class OpenGLRendererAPI : public RendererAPI
{
public:
    virtual void init() override;

    virtual void set_viewport(const uint32_t x,
        const uint32_t y,
        const uint32_t width,
        const uint32_t height) override;
    virtual void set_clear_color(const Vec4& color) override;
    virtual void clear() override;

    virtual void draw_indexed(const VertexArray& vertex_array,
        const uint32_t index_count = 0) override;
};

} // namespace pine
