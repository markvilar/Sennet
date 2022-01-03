#pragma once
#include "Sennet/Renderer/Texture.hpp"

namespace Sennet
{

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(const std::filesystem::path& imagePath);
    OpenGLTexture2D(const Image& image);

    ~OpenGLTexture2D();

    virtual uint32_t GetWidth() const override { return m_Width; }
    virtual uint32_t GetHeight() const override { return m_Height; }

    virtual RendererID GetRendererID() const override { return m_RendererID; }

    virtual void Bind(const uint32_t slot = 0) const override;
    virtual void Unbind() const override;

    virtual bool operator==(const Texture& other) const override;

private:
    RendererID m_RendererID;
    std::filesystem::path m_Source;
    Image m_Image;
    uint32_t m_Width;
    uint32_t m_Height;
};

} // namespace Sennet
