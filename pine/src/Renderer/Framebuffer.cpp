#include "Pine/Renderer/Framebuffer.hpp"

#include "Pine/Pch.hpp"
#include "Pine/Platform/OpenGL/OpenGLFramebuffer.hpp"
#include "Pine/Renderer/Renderer.hpp"

namespace Pine
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

} // namespace Pine
