#include "pine/renderer/vertex_array.hpp"

#include "pine/pch.hpp"
#include "pine/platform/opengl/vertex_array.hpp"
#include "pine/renderer/renderer.hpp"

namespace pine
{

std::unique_ptr<VertexArray> VertexArray::Create()
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLVertexArray>();
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

} // namespace pine
