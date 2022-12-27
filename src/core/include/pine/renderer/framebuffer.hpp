#pragma once

#include <cstdint>
#include <memory>

#include "pine/core/common.hpp"
#include "pine/renderer/renderer_api.hpp"

namespace pine
{

struct FramebufferSpecs
{
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t samples = 1;

    bool swapchain_target = false;

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

    virtual const FramebufferSpecs& get_specification() const = 0;
    static std::unique_ptr<Framebuffer> create(
        const FramebufferSpecs& specs);
};

} // namespace pine
