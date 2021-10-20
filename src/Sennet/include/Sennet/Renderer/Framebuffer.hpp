#pragma once

#include "Sennet/Core/Base.hpp"
#include "Sennet/Renderer/RendererAPI.hpp"

namespace Sennet
{

struct FramebufferSpecification
{
    uint32_t Width;
    uint32_t Height;
    // TODO: FramebufferFormat
    uint32_t Samples = 1;

    bool SwapChainTarget = false;
};

class Framebuffer
{
public:
    virtual ~Framebuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual RendererID GetColorAttachmentRendererID() const = 0;

    virtual const FramebufferSpecification& GetSpecification() const = 0;
    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
};

} // namespace Sennet
