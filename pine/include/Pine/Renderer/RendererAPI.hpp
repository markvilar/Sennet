#pragma once

#include <memory>

#include "Pine/Core/Base.hpp"
#include "Pine/Renderer/VertexArray.hpp"
#include "Pine/Utils/Math.hpp"

namespace Pine
{

using RendererID = uint32_t;

class RendererAPI
{
public:
    enum class API
    {
        None = 0,
        OpenGL = 1
    };

public:
    virtual ~RendererAPI() = default;
    virtual void Init() = 0;

    virtual void SetViewport(const uint32_t x, const uint32_t y,
        const uint32_t width, const uint32_t height) = 0;
    virtual void SetClearColor(const Vec4& color) = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(const VertexArray& vertexArray,
        const uint32_t indexCount = 0) = 0;

    inline static API GetAPI() { return s_API; }
    static std::unique_ptr<RendererAPI> Create();

private:
    static API s_API;
};

} // namespace Pine
