#include "pine/renderer/framebuffer.hpp"

#include "pine/platform/opengl/framebuffer.hpp"
#include "pine/renderer/renderer.hpp"
#include "pine/pch.hpp"

namespace pine
{

std::unique_ptr<Framebuffer> Framebuffer::Create(
    const Framebuffer::Specification& specs)
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
