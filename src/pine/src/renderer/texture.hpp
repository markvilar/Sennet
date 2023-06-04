#pragma once

#include "pine/common.hpp"

#include "image.hpp"
#include "types.hpp"

#include <filesystem>
#include <memory>

namespace pine
{

class Texture
{
public:
    virtual ~Texture() = default;

    virtual uint32_t get_width() const = 0;
    virtual uint32_t get_height() const = 0;

    virtual RendererID get_renderer_id() const = 0;

    virtual void bind(const uint32_t slot = 0) const = 0;
    virtual void unbind() const = 0;

    virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture
{
public:
    static std::unique_ptr<Texture2D> create(
        const std::filesystem::path& filepath);
    static std::unique_ptr<Texture2D> create(const Image& image);
};

} // namespace pine
