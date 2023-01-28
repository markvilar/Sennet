#include "pine/renderer/renderer_api.hpp"

#include "pine/pch.hpp"
#include "pine/platform/opengl/renderer_api.hpp"

namespace pine
{
RendererAPI::API RendererAPI::s_api = RendererAPI::API::OpenGL;

std::unique_ptr<RendererAPI> RendererAPI::create()
{
    switch (s_api)
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::None is currently not \
				supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLRendererAPI>();
    }

    PINE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

} // namespace pine
