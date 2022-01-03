#pragma once

#include <glm/glm.hpp>

#include "Sennet/Core/Base.hpp"
#include "Sennet/Renderer/VertexArray.hpp"

namespace Sennet
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
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(
        const Ref<VertexArray>& vertexArray, const uint32_t indexCount = 0) = 0;

    inline static API GetAPI() { return s_API; }
    static Scope<RendererAPI> Create();

private:
    static API s_API;
};

} // namespace Sennet
