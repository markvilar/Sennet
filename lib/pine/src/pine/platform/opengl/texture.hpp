#pragma once

#include "pine/renderer/texture.hpp"

namespace pine {

class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(const std::filesystem::path& imagePath);
    OpenGLTexture2D(const Image& image);

    ~OpenGLTexture2D();

    virtual uint32_t get_width() const override { return width; }
    virtual uint32_t get_height() const override { return height; }

    virtual RendererID get_renderer_id() const override { return renderer_id; }

    virtual void bind(const uint32_t slot = 0) const override;
    virtual void unbind() const override;

    virtual bool operator==(const Texture& other) const override;

private:
    RendererID renderer_id;
    uint32_t width;
    uint32_t height;
};

} // namespace pine
