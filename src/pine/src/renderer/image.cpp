#include "image.hpp"

#define STBI_NO_SIMD
#define STBI_NO_GIF
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

#include "pine/assert.hpp"
#include "pine/pch.hpp"

namespace pine
{

constexpr ImageFormat default_image_format(const int channels)
{
    switch (channels)
    {
    case 1:
        return ImageFormat::GRAY;
    case 2:
        return ImageFormat::GRAY_ALPHA;
    case 3:
        return ImageFormat::RGB;
    case 4:
        return ImageFormat::RGBA;
    }
    PINE_CORE_ASSERT(false, "Invalid number of image channels.");
    return ImageFormat::GRAY;
}

constexpr ImageFileFormat parse_image_file_format(
    const std::string_view file_extension)
{
    if (file_extension == ".png")
    {
        return ImageFileFormat::PNG;
    }
    else if (file_extension == ".bmp")
    {
        return ImageFileFormat::BMP;
    }
    else if (file_extension == ".tga")
    {
        return ImageFileFormat::TGA;
    }
    else if (file_extension == ".jpg")
    {
        return ImageFileFormat::JPG;
    }
    else
    {
        PINE_CORE_ASSERT(false, "Invalid image file format.");
        return ImageFileFormat::PNG;
    }
}

constexpr uint32_t get_format_channel_count(const ImageFormat format)
{
    switch (format)
    {
    case ImageFormat::INVALID:
        return 0;
    case ImageFormat::GRAY:
        return 1;
    case ImageFormat::GRAY_ALPHA:
        return 2;
    case ImageFormat::RGB:
        return 3;
    case ImageFormat::BGR:
        return 3;
    case ImageFormat::RGBA:
        return 4;
    case ImageFormat::BGRA:
        return 4;
    }
    PINE_CORE_ASSERT(false, "Invalid image format.");
    return 0;
}

Image::Image(const uint8_t* data,
    const uint32_t width,
    const uint32_t height,
    const ImageFormat format)
    : width(width),
      height(height),
      format(format)
{
    const auto channels = get_format_channel_count(format);
    buffer = std::vector<uint8_t>(data, data + width * height * channels);
}

Image read_image(const std::filesystem::path& filepath, const bool flip)
{
    int width, height, channels = 0;

    stbi_set_flip_vertically_on_load(flip);

    const auto data
        = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
    const auto format = default_image_format(channels);

    return Image(data,
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height),
        format);
}

Image read_image(const std::filesystem::path& filepath,
    const ImageFormat format,
    const bool flip)
{
    int width, height, channels = 0;
    auto desired_channels = get_format_channel_count(format);

    stbi_set_flip_vertically_on_load(flip);

    const auto data = stbi_load(filepath.string().c_str(),
        &width,
        &height,
        &channels,
        static_cast<int>(desired_channels));

    return Image(data,
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height),
        format);
}

bool write_image(const std::filesystem::path& filepath,
    const Image& image,
    const bool flip)
{
    const auto file_extension = filepath.extension();
    const auto file_format = parse_image_file_format(file_extension.string().c_str());

    stbi_set_flip_vertically_on_load(flip);

    const auto channels = get_format_channel_count(image.get_format());
    const auto write_result = [filepath, image, channels, file_format]()
    {
        switch (file_format)
        {
        case ImageFileFormat::JPG:
            return stbi_write_jpg(filepath.string().c_str(),
                static_cast<int>(image.get_width()),
                static_cast<int>(image.get_height()),
                static_cast<int>(channels),
                image.get_buffer().data(),
                100);
        case ImageFileFormat::PNG:
            return stbi_write_png(filepath.string().c_str(),
                static_cast<int>(image.get_width()),
                static_cast<int>(image.get_height()),
                static_cast<int>(channels),
                image.get_buffer().data(),
                static_cast<int>(image.get_width() * channels));
        case ImageFileFormat::BMP:
            return stbi_write_bmp(filepath.string().c_str(),
                static_cast<int>(image.get_width()),
                static_cast<int>(image.get_height()),
                static_cast<int>(channels),
                image.get_buffer().data());
        case ImageFileFormat::TGA:
            return stbi_write_tga(filepath.string().c_str(),
                static_cast<int>(image.get_width()),
                static_cast<int>(image.get_height()),
                static_cast<int>(channels),
                image.get_buffer().data());
        }
        return 0;
    }();

    return write_result;
}

} // namespace pine
