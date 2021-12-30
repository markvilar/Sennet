#pragma once

#include <filesystem>
#include <vector>

namespace Sennet
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
    UNKNOWN,
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
    uint32_t Width = 0;
    uint32_t Height = 0;
    ImageFormat Format = ImageFormat::UNKNOWN;
    std::vector<uint8_t> Buffer = {};

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

Image ReadImage(const std::filesystem::path& filepath, const bool flip = false);
Image ReadImage(const std::filesystem::path& filepath, const ImageFormat format,
    const bool flip = false);

bool WriteImage(const std::filesystem::path& filepath, const Image& image,
    const bool flip = false);

} // namespace Sennet
