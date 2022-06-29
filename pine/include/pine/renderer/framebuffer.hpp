#pragma once

#include <cstdint>
#include <memory>

#include "pine/core/common.hpp"
#include "pine/renderer/renderer_api.hpp"

namespace pine
{

class Framebuffer
{
public:
    struct Specification
    {
        uint32_t Width = 0;
        uint32_t Height = 0;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;

        // TODO: FramebufferFormat
    };

public:
    virtual ~Framebuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void Resize(const uint32_t width, const uint32_t height) = 0;

    virtual RendererID GetColorAttachmentRendererID() const = 0;

    virtual const Specification& GetSpecification() const = 0;
    static std::unique_ptr<Framebuffer> Create(const Specification& specs);
};

} // namespace pine
