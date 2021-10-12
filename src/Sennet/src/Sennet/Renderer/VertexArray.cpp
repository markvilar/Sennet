#include "Sennet/Renderer/VertexArray.hpp"
#include "Sennet/Pch.hpp"

#include "Sennet/Renderer/Renderer.hpp"

#include "Sennet/Platform/OpenGL/OpenGLVertexArray.hpp"

namespace Sennet
{

Ref<VertexArray> VertexArray::Create()
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SENNET_CORE_ASSERT(false, "Renderer API None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexArray>();
    }

    SENNET_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

} // namespace Sennet
