#include "Sennet/Platform/OpenGL/OpenGLTexture.hpp"
#include "Sennet/Pch.hpp"

#include "glad/glad.h"

namespace Sennet
{

GLenum SennetToOpenGL(const Texture::InternalFormat& internalFormat)
{
    GLenum glInternalFormat = 0;
    switch (internalFormat)
    {
    case Texture::InternalFormat::RGBA8:
        glInternalFormat = GL_RGBA8;
        break;
    case Texture::InternalFormat::RGB8:
        glInternalFormat = GL_RGB8;
        break;
    }
    SENNET_CORE_ASSERT(glInternalFormat, "Invalid OpenGL internal format.");
    return glInternalFormat;
}

GLenum SennetToOpenGL(const Texture::DataFormat& dataFormat)
{
    GLenum glDataFormat = 0;
    switch (dataFormat)
    {
    case Texture::DataFormat::RGBA:
        glDataFormat = GL_RGBA;
        break;
    case Texture::DataFormat::BGRA:
        glDataFormat = GL_BGRA;
        break;
    case Texture::DataFormat::RGB:
        glDataFormat = GL_RGB;
        break;
    case Texture::DataFormat::BGR:
        glDataFormat = GL_BGR;
        break;
    }
    SENNET_CORE_ASSERT(glDataFormat, "Invalid OpenGL data format.");
    return glDataFormat;
}

OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height,
    const InternalFormat internalFormat, const DataFormat dataFormat)
    : m_Source(""), m_Width(width), m_Height(height),
      m_InternalFormat(internalFormat), m_DataFormat(dataFormat)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID,
        1,
        SennetToOpenGL(m_InternalFormat),
        m_Width,
        m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Source(path)
{
    SENNET_CORE_ASSERT(false, "OpenGLTexture2D constructor not implemented.");
    // TODO: Implement image loader and use it here.
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_RendererID); }

void OpenGLTexture2D::SetData(void* data, const uint32_t& size)
{
    uint32_t bpp = 0;
    switch (m_DataFormat)
    {
    case DataFormat::RGBA:
        bpp = 4;
        break;
    case DataFormat::BGRA:
        bpp = 4;
        break;
    case DataFormat::RGB:
        bpp = 3;
        break;
    case DataFormat::BGR:
        bpp = 3;
        break;
    }
    SENNET_CORE_ASSERT(size == m_Width * m_Height * bpp,
        "Data must be entire texture.");
    glTextureSubImage2D(m_RendererID,
        0,
        0,
        0,
        m_Width,
        m_Height,
        SennetToOpenGL(m_DataFormat),
        GL_UNSIGNED_BYTE,
        data);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_RendererID);
}

void OpenGLTexture2D::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

bool OpenGLTexture2D::operator==(const Texture& other) const
{
    return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
};

} // namespace Sennet
