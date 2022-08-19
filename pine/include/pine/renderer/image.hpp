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
    using BufferType = std::vector<uint8_t>;

public:
    Image() = default;
    Image(const Image& image) = default;
    Image(Image&& image) = default;

    Image(const uint8_t* data, const uint32_t width, const uint32_t height,
        const ImageFormat format);

    ~Image() = default;

    Image& operator=(const Image& image) = default;
    Image& operator=(Image&& image) = default;

    uint32_t get_width() const { return width; }
    uint32_t get_height() const { return height; }
    ImageFormat get_format() const { return format; }
    const BufferType get_buffer() const { return buffer; }

private:
    uint32_t width = 0;
    uint32_t height = 0;
    ImageFormat format = ImageFormat::UNKNOWN;
    BufferType buffer = {};
};

// TODO: Return std::optional<Image>
Image read_image(const std::filesystem::path& filepath,
    const bool flip = false);
Image read_image(const std::filesystem::path& filepath,
    const ImageFormat format, const bool flip = false);

bool write_image(const std::filesystem::path& filepath, const Image& image,
    const bool flip = false);

} // namespace pine
