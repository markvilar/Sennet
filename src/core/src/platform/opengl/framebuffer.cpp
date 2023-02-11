#include "pine/platform/opengl/framebuffer.hpp"

#include <glad/glad.h>

#include "pine/pch.hpp"
#include "pine/renderer/types.hpp"

namespace pine
{

static constexpr uint32_t MAX_FRAMEBUFFER_SIZE = 8192;

// TODO: Configure based on attachment format, filter, and wrap
RendererID create_color_attachment([[maybe_unused]] const FramebufferAttachment&
                                       attachment,
    const uint32_t width,
    const uint32_t height)
{
    RendererID renderer_id;
    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
    glBindTexture(GL_TEXTURE_2D, renderer_id);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA8, // TODO: Convert to internal format
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        0,
        GL_RGBA, // TODO: Convert to data format
        GL_UNSIGNED_BYTE,
        nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0, // TODO: Configure texture type
        GL_TEXTURE_2D,
        renderer_id,
        0);
    return renderer_id;
}

// TODO: Configure based on attachment format, filter, and wrap
RendererID create_depth_attachment([[maybe_unused]] const FramebufferAttachment&
                                       attachment,
    const uint32_t width,
    const uint32_t height)
{
    RendererID renderer_id;
    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
    glBindTexture(GL_TEXTURE_2D, renderer_id);
    glTexStorage2D(GL_TEXTURE_2D,
        1,
        GL_DEPTH24_STENCIL8,
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height));
    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT, // TODO: Configure texture type
        GL_TEXTURE_2D,
        renderer_id,
        0);
    return renderer_id;
}

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& specs)
    : specification(specs)
{
    invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
    glDeleteFramebuffers(1, &renderer_id);
    glDeleteTextures(1, &color_attachment);
    glDeleteTextures(1, &depth_attachment);
}

void OpenGLFramebuffer::invalidate()
{
    if (renderer_id)
    {
        glDeleteFramebuffers(1, &renderer_id);
        glDeleteTextures(1, &color_attachment);
        glDeleteTextures(1, &depth_attachment);
    }

    // Create new framebuffer
    glCreateFramebuffers(1, &renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer_id);

    if (is_color_format(specification.color_attachment.format))
    {
        color_attachment
            = create_color_attachment(specification.color_attachment,
                specification.width,
                specification.height);
    }
    else
    {
        PINE_INFO("Framebuffer: Invalid color attachment format.");
    }

    if (is_depth_format(specification.depth_attachment.format))
    {
        depth_attachment
            = create_depth_attachment(specification.depth_attachment,
                specification.width,
                specification.height);
    }
    else
    {
        PINE_INFO("Framebuffer: Invalid depth attachment format.");
    }

    // Check framebuffer completion
    PINE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER)
            == GL_FRAMEBUFFER_COMPLETE,
        "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, renderer_id);
    glViewport(0,
        0,
        static_cast<GLsizei>(specification.width),
        static_cast<GLsizei>(specification.height));
}

void OpenGLFramebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFramebuffer::resize(const uint32_t width, const uint32_t height)
{
    if (width == 0 || height == 0 || width > MAX_FRAMEBUFFER_SIZE
        || height > MAX_FRAMEBUFFER_SIZE)
    {
        PINE_CORE_WARN("Attempted to resize framebuffer to {0}, {1}",
            width,
            height);
        return;
    }

    specification.width = width;
    specification.height = height;

    invalidate();
}

} // namespace pine
