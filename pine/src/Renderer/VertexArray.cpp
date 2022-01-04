#include "Pine/Pch.hpp"
#include "Pine/Renderer/VertexArray.hpp"

#include "Pine/Platform/OpenGL/OpenGLVertexArray.hpp"
#include "Pine/Renderer/Renderer.hpp"

namespace Pine
{

Ref<VertexArray> VertexArray::Create()
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexArray>();
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

} // namespace Pine
