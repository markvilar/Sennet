#pragma once

#include "Sennet/Renderer/Texture.hpp"

namespace Sennet
{

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(const uint32_t& width,
        const uint32_t& height,
        const InternalFormat internalFormat,
        const DataFormat dataFormat);
    OpenGLTexture2D(const std::string& path);
    ~OpenGLTexture2D();

    virtual uint32_t GetWidth() const override { return m_Width; }
    virtual uint32_t GetHeight() const override { return m_Height; }

    virtual RendererID GetRendererID() const override { return m_RendererID; }

    virtual void SetData(void* data, const uint32_t& size) override;

    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Unbind() const override;

    virtual bool operator==(const Texture& other) const override;

private:
    std::string m_Source;
    uint32_t m_Width;
    uint32_t m_Height;
    RendererID m_RendererID;
    InternalFormat m_InternalFormat;
    DataFormat m_DataFormat;
};

} // namespace Sennet
