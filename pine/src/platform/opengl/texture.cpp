#include "pine/platform/opengl/texture.hpp"
#include "pine/pch.hpp"

#include <filesystem>
#include <fstream>

#include <glad/glad.h>

namespace pine
{

GLenum to_opengl_internal_format(const ImageFormat& image_format)
{
    const auto texture_format = static_cast<TextureFormat>(image_format);
    const auto internal_format = [texture_format]()
    {
        switch (texture_format)
        {
        case TextureFormat::UNKNOWN:
            return GL_INVALID_ENUM;
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
        }
        return GL_INVALID_ENUM;
    }();

    PINE_CORE_ASSERT(internal_format, "Invalid OpenGL data format.");
    return static_cast<GLenum>(internal_format);
}

GLenum to_opengl_data_format(const ImageFormat& image_format)
{
    const auto texture_format = static_cast<TextureFormat>(image_format);
    const auto internal_format = [texture_format]()
    {
        switch (texture_format)
        {
        case TextureFormat::UNKNOWN:
            return GL_INVALID_ENUM;
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
        }
        return GL_INVALID_ENUM;
    }();

    PINE_CORE_ASSERT(internal_format, "Invalid OpenGL data format.");
    return static_cast<GLenum>(internal_format);
}

OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& image_path)
    : OpenGLTexture2D(read_image(image_path))
{
    m_source = image_path;
}

OpenGLTexture2D::OpenGLTexture2D(const Image& image)
    : m_source(""), m_width(image.get_width()), m_height(image.get_height())
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);

    glTextureStorage2D(m_renderer_id,
        1,
        to_opengl_internal_format(image.get_format()),
        static_cast<GLsizei>(image.get_width()),
        static_cast<GLsizei>(image.get_height()));

    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(m_renderer_id,
        0,
        0,
        0,
        static_cast<GLsizei>(image.get_width()),
        static_cast<GLsizei>(image.get_height()),
        to_opengl_data_format(image.get_format()),
        GL_UNSIGNED_BYTE,
        static_cast<const void*>(image.get_buffer().data()));
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_renderer_id); }

void OpenGLTexture2D::bind(const uint32_t slot) const
{
    glBindTextureUnit(slot, m_renderer_id);
}

void OpenGLTexture2D::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

bool OpenGLTexture2D::operator==(const Texture& other) const
{
    return m_renderer_id
        == reinterpret_cast<const OpenGLTexture2D&>(other).m_renderer_id;
}

} // namespace pine
