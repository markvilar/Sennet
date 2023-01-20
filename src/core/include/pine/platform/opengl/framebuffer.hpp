#pragma once

#include "pine/renderer/framebuffer.hpp"

namespace pine
{

class OpenGLFramebuffer : public Framebuffer
{
public:
    OpenGLFramebuffer(const FramebufferSpecs& specs);
    virtual ~OpenGLFramebuffer();

    void invalidate();

    virtual void bind() override;
    virtual void unbind() override;

    virtual void resize(const uint32_t width, const uint32_t height) override;

    virtual RendererID get_color_attachment_renderer_id() const override
    {
        return color_attachment;
    }

    virtual const FramebufferSpecs& get_specification() const override
    {
        return specification;
    }

private:
    RendererID renderer_id = 0;
    RendererID color_attachment = 0;
    RendererID depth_attachment = 0;
    FramebufferSpecs specification;
};

} // namespace pine
