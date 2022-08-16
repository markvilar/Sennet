#include "pine/renderer/framebuffer.hpp"

#include "pine/pch.hpp"
#include "pine/platform/opengl/framebuffer.hpp"
#include "pine/renderer/renderer.hpp"

namespace pine
{

std::unique_ptr<Framebuffer> Framebuffer::create(
    const FramebufferSpecification& specs)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::None is currently not \
				supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLFramebuffer>(specs);
    }

    PINE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

} // namespace pine
