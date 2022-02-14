#pragma once

#include <memory>

#include "Pine/Renderer/RendererAPI.hpp"

namespace Pine
{

class OpenGLRendererAPI : public RendererAPI
{
public:
    virtual void Init() override;

    virtual void SetViewport(const uint32_t x, const uint32_t y,
        const uint32_t width, const uint32_t height) override;
    virtual void SetClearColor(const Vec4& color) override;
    virtual void Clear() override;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray,
        const uint32_t indexCount = 0) override;
};

} // namespace Pine
