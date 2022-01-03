#pragma once

#include "Sennet/Renderer/Framebuffer.hpp"

namespace Sennet
{

class OpenGLFramebuffer : public Framebuffer
{
public:
    OpenGLFramebuffer(const Framebuffer::Specification& specs);
    virtual ~OpenGLFramebuffer();

    void Invalidate();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual void Resize(uint32_t width, uint32_t height) override;

    virtual RendererID GetColorAttachmentRendererID() const override
    {
        return m_ColorAttachment;
    }

    virtual const Framebuffer::Specification& GetSpecification() const override
    {
        return m_Specification;
    }

private:
    RendererID m_RendererID = 0;
    RendererID m_ColorAttachment = 0;
    RendererID m_DepthAttachment = 0;
    Framebuffer::Specification m_Specification;
};

} // namespace Sennet
