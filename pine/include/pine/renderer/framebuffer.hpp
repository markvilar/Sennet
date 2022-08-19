#pragma once

#include <cstdint>
#include <memory>

#include "pine/core/common.hpp"
#include "pine/renderer/renderer_api.hpp"

namespace pine
{

struct FramebufferSpecification
{
    uint32_t Width = 0;
    uint32_t Height = 0;
    uint32_t Samples = 1;

    bool SwapChainTarget = false;

    // TODO: FramebufferFormat
};

class Framebuffer
{
public:
    virtual ~Framebuffer() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual void resize(const uint32_t width, const uint32_t height) = 0;

    virtual RendererID get_color_attachment_renderer_id() const = 0;

    virtual const FramebufferSpecification& get_specification() const = 0;
    static std::unique_ptr<Framebuffer> create(
        const FramebufferSpecification& specs);
};

} // namespace pine
