#include "Sennet/Renderer/Framebuffer.hpp"
#include "Sennet/Pch.hpp"

#include "Sennet/Renderer/Renderer.hpp"

#include "Sennet/Platform/OpenGL/OpenGLFramebuffer.hpp"

namespace Sennet
{

Ref<Framebuffer> Framebuffer::Create(const Framebuffer::Specification& specs)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SENNET_CORE_ASSERT(false, "RendererAPI::None is currently not \
				supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLFramebuffer>(specs);
    }

    SENNET_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

} // namespace Sennet
