#include "Sennet/Platform/OpenGL/OpenGLTexture.hpp"
#include "Sennet/Pch.hpp"

#include "glad/glad.h"

#include <filesystem>
#include <fstream>

namespace Sennet
{

GLenum OpenGLInternalFormat(const ImageFormat& imageFormat)
{
    const auto textureFormat = static_cast<TextureFormat>(imageFormat);
    const auto internalFormat = [textureFormat]() {
        switch (textureFormat)
        {
        case TextureFormat::RED:
            return GL_R8;
        case TextureFormat::RG:
            return GL_RG8;
        case TextureFormat::RGB:
            return GL_RGB8;
        case TextureFormat::BGR:
            return GL_RGB8;
        case TextureFormat::RGBA:
            return GL_RGBA8;
        case TextureFormat::BGRA:
            return GL_RGBA8;
        default:
            return 0;
        }
    }();

    SENNET_CORE_ASSERT(internalFormat, "Invalid OpenGL data format.");
    return internalFormat;
}

GLenum OpenGLDataFormat(const ImageFormat& imageFormat)
{
    const auto textureFormat = static_cast<TextureFormat>(imageFormat);
    const auto internalFormat = [textureFormat]() {
        switch (textureFormat)
        {
        case TextureFormat::RED:
            return GL_RED;
        case TextureFormat::RG:
            return GL_RG;
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::BGR:
            return GL_BGR;
        case TextureFormat::RGBA:
            return GL_RGBA;
        case TextureFormat::BGRA:
            return GL_BGRA;
        default:
            return 0;
        }
    }();

    SENNET_CORE_ASSERT(internalFormat, "Invalid OpenGL data format.");
    return internalFormat;
}

OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& imagePath)
    : OpenGLTexture2D(ReadImage(imagePath))
{
    m_Source = imagePath;
}

OpenGLTexture2D::OpenGLTexture2D(const Image& image)
    : m_Source(""), m_Width(image.Width), m_Height(image.Height)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

    glTextureStorage2D(m_RendererID,
        1,
        OpenGLInternalFormat(image.Format),
        image.Width,
        image.Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(m_RendererID,
        0,
        0,
        0,
        image.Width,
        image.Height,
        OpenGLDataFormat(image.Format),
        GL_UNSIGNED_BYTE,
        static_cast<const void*>(image.Buffer.data()));
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_RendererID); }

void OpenGLTexture2D::Bind(const uint32_t slot) const
{
    glBindTextureUnit(slot, m_RendererID);
}

void OpenGLTexture2D::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

bool OpenGLTexture2D::operator==(const Texture& other) const
{
    return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
};

} // namespace Sennet
