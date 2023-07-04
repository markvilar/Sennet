#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

namespace pine
{

enum class ImageFileFormat
{
    JPG,
    PNG,
    BMP,
    TGA
};

enum class ImageFormat
{
    INVALID,
    GRAY,
    GRAY_ALPHA,
    RGB,
    BGR,
    RGBA,
    BGRA
};

struct Image
{
    using BufferType = std::vector<uint8_t>;

public:
    Image() = default;
    Image(const Image& image) = default;
    Image(Image&& image) = default;

    Image(const uint8_t* data,
        const uint32_t width,
        const uint32_t height,
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
    ImageFormat format = ImageFormat::INVALID;
    BufferType buffer = {};
};

Image read_image(const std::filesystem::path& filepath,
    const bool flip = false);
Image read_image(const std::filesystem::path& filepath,
    const ImageFormat format,
    const bool flip = false);

bool write_image(const std::filesystem::path& filepath,
    const Image& image,
    const bool flip = false);

} // namespace pine
