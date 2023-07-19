#pragma once

#include <cstdint>
#include <memory>

#include "pine/renderer/renderer_api.hpp"
#include "pine/renderer/texture.hpp"

namespace pine {

struct FramebufferAttachment {
    TextureFormat format = TextureFormat::INVALID;
    TextureFilter filter = TextureFilter::NONE;
    TextureWrap wrap = TextureWrap::NONE;
};

struct FramebufferSpecs {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t samples = 1;

    bool swapchain_target = false;

    FramebufferAttachment color_attachment;
    FramebufferAttachment depth_attachment;
};

class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual void resize(const uint32_t width, const uint32_t height) = 0;

    virtual RendererID get_color_attachment_renderer_id() const = 0;

    virtual const FramebufferSpecs& get_specification() const = 0;
};

std::unique_ptr<Framebuffer> create_framebuffer(const FramebufferSpecs& specs);

} // namespace pine
