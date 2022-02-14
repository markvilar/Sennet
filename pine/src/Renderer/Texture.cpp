#include "Pine/Renderer/Texture.hpp"

#include "Pine/Pch.hpp"
#include "Pine/Platform/OpenGL/OpenGLTexture.hpp"
#include "Pine/Renderer/Renderer.hpp"

namespace Pine
{

std::shared_ptr<Texture2D> Texture2D::Create(
    const std::filesystem::path& filePath)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
			supported!");
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLTexture2D>(filePath);
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::Create(const Image& image)
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
            supported!");
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLTexture2D>(image);
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

} // namespace Pine
