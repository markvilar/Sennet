#include "pine/renderer/texture.hpp"

#include "pine/pch.hpp"
#include "pine/platform/opengl/texture.hpp"
#include "pine/renderer/renderer.hpp"

namespace pine
{

std::unique_ptr<Texture2D> Texture2D::create(
    const std::filesystem::path& filepath)
{
    switch (Renderer::get_api())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
			supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLTexture2D>(filepath);
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

std::unique_ptr<Texture2D> Texture2D::create(const Image& image)
{
    switch (Renderer::get_api())
    {
    case RendererAPI::API::None:
        PINE_CORE_ASSERT(false, "Renderer API None is currently not \
            supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_unique<OpenGLTexture2D>(image);
    }

    PINE_CORE_ASSERT(false, "Unknown Renderer API.");
    return nullptr;
}

} // namespace pine
