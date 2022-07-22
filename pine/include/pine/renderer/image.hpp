#pragma once

#include <filesystem>
#include <vector>

namespace pine
{

enum class TextureFormat : uint8_t
{
    UNKNOWN,
    RED,
    RG,
    RGB,
    BGR,
    RGBA,
    BGRA
};

enum class ImageFileFormat : uint8_t
{
    JPG,
    PNG,
    BMP,
    TGA
};

enum class ImageFormat : uint8_t
{
    UNKNOWN = static_cast<uint8_t>(TextureFormat::UNKNOWN),
    GRAY = static_cast<uint8_t>(TextureFormat::RED),
    GRAY_ALPHA = static_cast<uint8_t>(TextureFormat::RG),
    RGB = static_cast<uint8_t>(TextureFormat::RGB),
    BGR = static_cast<uint8_t>(TextureFormat::BGR),
    RGBA = static_cast<uint8_t>(TextureFormat::RGBA),
    BGRA = static_cast<uint8_t>(TextureFormat::BGRA)
};

struct Image
{
    uint32_t width = 0;
    uint32_t height = 0;
    ImageFormat format = ImageFormat::UNKNOWN;
    std::vector<uint8_t> buffer = {};

public:
    Image() = default;
    Image(const Image& image) = default;
    Image(Image&& image) = default;

    Image(const uint8_t* data, const uint32_t width, const uint32_t height,
        const ImageFormat format);

    ~Image() = default;

    Image& operator=(const Image& image) = default;
    Image& operator=(Image&& image) = default;
};

// TODO: Return std::optional<Image>
Image read_image(const std::filesystem::path& filepath, 
    const bool flip = false);
Image read_image(const std::filesystem::path& filepath, 
    const ImageFormat format, const bool flip = false);

bool write_image(const std::filesystem::path& filepath, const Image& image,
    const bool flip = false);

} // namespace pine
