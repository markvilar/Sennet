#pragma once

#include <filesystem>
#include <memory>

#include "pine/core/common.hpp"
#include "pine/renderer/image.hpp"
#include "pine/renderer/renderer_api.hpp"

namespace pine
{

class Texture
{
public:
    virtual ~Texture() = default;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual RendererID GetRendererID() const = 0;

    virtual void Bind(const uint32_t slot = 0) const = 0;
    virtual void Unbind() const = 0;

    virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture
{
public:
    static std::unique_ptr<Texture2D> Create(
        const std::filesystem::path& filePath);
    static std::unique_ptr<Texture2D> Create(const Image& image);
};

} // namespace pine
