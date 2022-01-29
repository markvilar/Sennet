#include "Pine/Renderer/Framebuffer.hpp"

#include "Pine/Pch.hpp"
#include "Pine/Platform/OpenGL/OpenGLFramebuffer.hpp"
#include "Pine/Renderer/Renderer.hpp"

namespace Pine
{

Ref<Framebuffer> Framebuffer::Create(const Framebuffer::Specification& specs)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "RendererAPI::None is currently not \
				supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLFramebuffer>(specs);
    }

    PINE_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

} // namespace Pine
