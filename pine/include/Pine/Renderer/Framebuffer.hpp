#pragma once

#include "Pine/Core/Base.hpp"
#include "Pine/Renderer/RendererAPI.hpp"

namespace Pine
{

class Framebuffer
{
public:
    struct Specification
    {
        uint32_t Width;
        uint32_t Height;
        // TODO: FramebufferFormat
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

public:
    virtual ~Framebuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void Resize(const uint32_t width, const uint32_t height) = 0;

    virtual RendererID GetColorAttachmentRendererID() const = 0;

    virtual const Specification& GetSpecification() const = 0;
    static Ref<Framebuffer> Create(const Specification& specs);
};

} // namespace Pine
