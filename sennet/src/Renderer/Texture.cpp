#include "Sennet/Renderer/Texture.hpp"
#include "Sennet/Pch.hpp"

#include "Sennet/Renderer/Renderer.hpp"

#include "Sennet/Platform/OpenGL/OpenGLTexture.hpp"

namespace Sennet
{

Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filePath)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SENNET_CORE_ASSERT(false, "Renderer API None is currently not \
			supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLTexture2D>(filePath);
    }

    SENNET_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

Ref<Texture2D> Texture2D::Create(const Image& image)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        SENNET_CORE_ASSERT(false, "Renderer API None is currently not \
            supported!");
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLTexture2D>(image);
    }

    SENNET_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

} // namespace Sennet
