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
        return m_color_attachment;
    }

    virtual const FramebufferSpecs& get_specification() const override
    {
        return m_specification;
    }

private:
    RendererID m_renderer_id = 0;
    RendererID m_color_attachment = 0;
    RendererID m_depth_attachment = 0;
    FramebufferSpecs m_specification;
};

} // namespace pine
