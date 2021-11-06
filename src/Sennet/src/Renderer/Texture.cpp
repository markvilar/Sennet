#include "Sennet/Renderer/Texture.hpp"
#include "Sennet/Pch.hpp"

#include "Sennet/Renderer/Renderer.hpp"

#include "Sennet/Platform/OpenGL/OpenGLTexture.hpp"

namespace Sennet
{

Ref<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SENNET_CORE_ASSERT(false, "Renderer API None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLTexture2D>(width,
            height,
            InternalFormat::RGBA8,
            DataFormat::RGBA);
    }

    SENNET_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

Ref<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height,
    const InternalFormat internalFormat, const DataFormat dataFormat)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SENNET_CORE_ASSERT(false, "Renderer API None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLTexture2D>(width,
            height,
            internalFormat,
            dataFormat);
    }

    SENNET_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::string& path)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SENNET_CORE_ASSERT(false, "Renderer API None is currently not \
				supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLTexture2D>(path);
    }

    SENNET_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

} // namespace Sennet
