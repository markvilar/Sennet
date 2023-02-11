#include "pine/platform/opengl/texture.hpp"

#include <filesystem>
#include <fstream>

#include <glad/glad.h>

#include "pine/pch.hpp"
#include "pine/platform/opengl/utilities.hpp"

namespace pine
{

OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& image_path)
    : OpenGLTexture2D(read_image(image_path))
{
}

OpenGLTexture2D::OpenGLTexture2D(const Image& image)
    : width(image.get_width()),
      height(image.get_height())
{
    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);

    glTextureStorage2D(renderer_id,
        1,
        opengl::to_opengl_internal(image.get_format()),
        static_cast<GLsizei>(image.get_width()),
        static_cast<GLsizei>(image.get_height()));

    glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(renderer_id,
        0,
        0,
        0,
        static_cast<GLsizei>(image.get_width()),
        static_cast<GLsizei>(image.get_height()),
        opengl::to_opengl(image.get_format()),
        GL_UNSIGNED_BYTE,
        static_cast<const void*>(image.get_buffer().data()));
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &renderer_id); }

void OpenGLTexture2D::bind(const uint32_t slot) const
{
    glBindTextureUnit(slot, renderer_id);
}

void OpenGLTexture2D::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

bool OpenGLTexture2D::operator==(const Texture& other) const
{
    return renderer_id
        == reinterpret_cast<const OpenGLTexture2D&>(other).renderer_id;
}

} // namespace pine
