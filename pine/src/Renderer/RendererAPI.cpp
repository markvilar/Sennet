#include "Pine/Renderer/RendererAPI.hpp"

#include "Pine/Pch.hpp"
#include "Pine/Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Pine
{

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

Scope<RendererAPI> RendererAPI::Create()
{
    switch (s_API)
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::None is currently not \
				supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateScope<OpenGLRendererAPI>();
    }

    PINE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

} // namespace Pine
