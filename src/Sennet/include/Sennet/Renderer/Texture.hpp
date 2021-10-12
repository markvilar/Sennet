#pragma once

#include <string>

#include "Sennet/Core/Base.hpp"
#include "Sennet/Renderer/RendererAPI.hpp"

namespace Sennet
{

class Texture
{
public:
    enum class InternalFormat : uint8_t
    {
        None = 0,
        RGBA8 = 1,
        RGB8 = 2,
    };

    enum class DataFormat : uint8_t
    {
        None = 0,
        RGBA = 1,
        BGRA = 2,
        RGB = 3,
        BGR = 4,
    };

    virtual ~Texture() = default;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual RendererID GetRendererID() const = 0;

    virtual void SetData(void* data, const uint32_t& size) = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() const = 0;

    virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture
{
public:
    static Ref<Texture2D> Create(const uint32_t width, const uint32_t height);
    static Ref<Texture2D> Create(const uint32_t width,
        const uint32_t height,
        const InternalFormat internalFormat,
        const DataFormat dataFormat);
    static Ref<Texture2D> Create(const std::string& path);
};

} // namespace Sennet
