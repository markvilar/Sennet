#pragma once

#include <memory>

#include "Pine/Renderer/RendererAPI.hpp"
#include "Pine/Utils/Math.hpp"

namespace Pine
{

class RenderCommand
{
public:
    inline static void Init() { s_RendererAPI->Init(); }

    inline static void SetViewport(const uint32_t x, const uint32_t y,
        const uint32_t width, const uint32_t height)
    {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

    inline static void SetClearColor(const Vec4& color)
    {
        s_RendererAPI->SetClearColor(color);
    }

    inline static void Clear() { s_RendererAPI->Clear(); }

    inline static void DrawIndexed(
        const VertexArray& vertexArray, const uint32_t indexCount = 0)
    {
        s_RendererAPI->DrawIndexed(vertexArray, indexCount);
    }

private:
    static std::unique_ptr<RendererAPI> s_RendererAPI;
};

} // namespace Pine
