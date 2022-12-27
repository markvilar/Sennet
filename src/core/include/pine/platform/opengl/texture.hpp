#pragma once

#include "pine/renderer/texture.hpp"

namespace pine
{

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(const std::filesystem::path& imagePath);
    OpenGLTexture2D(const Image& image);

    ~OpenGLTexture2D();

    virtual uint32_t get_width() const override { return m_width; }
    virtual uint32_t get_height() const override { return m_height; }

    virtual RendererID get_renderer_id() const override
    {
        return m_renderer_id;
    }

    virtual void bind(const uint32_t slot = 0) const override;
    virtual void unbind() const override;

    virtual bool operator==(const Texture& other) const override;

private:
    RendererID m_renderer_id;
    std::filesystem::path m_source;
    Image m_image;
    uint32_t m_width;
    uint32_t m_height;
};

} // namespace pine
