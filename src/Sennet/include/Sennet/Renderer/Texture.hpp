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
        RGBA8,
        RGB8,
    };

    enum class DataFormat : uint8_t
    {
        RGBA,
        BGRA,
        RGB,
        BGR,
    };

    virtual ~Texture() = default;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual RendererID GetRendererID() const = 0;

    virtual void SetData(const void* data, const uint32_t size) = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() const = 0;

    virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture
{
public:
    static Ref<Texture2D> Create(const uint32_t width, const uint32_t height);

    static Ref<Texture2D> Create(const uint32_t width, const uint32_t height,
        const InternalFormat internalFormat, const DataFormat dataFormat);

    static Ref<Texture2D> Create(const std::string& path);
};

} // namespace Sennet
