#include "Sennet/Renderer/Image.hpp"
#include "Sennet/Pch.hpp"

#define STBI_NO_SIMD
#define STBI_NO_GIF
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#include <fstream>

namespace Sennet
{

ImageFormat ParseImageFormat(const uint8_t channels)
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
    default:
        return ImageFormat::RGBA;
    }
}

ImageFileFormat ParseImageFileFormat(const std::string_view fileExtension)
{
    if (fileExtension == ".png")
    {
        return ImageFileFormat::PNG;
    }
    else if (fileExtension == ".bmp")
    {
        return ImageFileFormat::BMP;
    }
    else if (fileExtension == ".tga")
    {
        return ImageFileFormat::TGA;
    }
    else if (fileExtension == ".jpg")
    {
        return ImageFileFormat::JPG;
    }
    else
    {
        return ImageFileFormat::UNKNOWN;
    }
}

Image::Image(const uint8_t* data, const uint32_t width, const uint32_t height,
    const uint8_t channels, const ImageFormat format)
    : Buffer(data, data + width * height * channels), Width(width),
      Height(height), Channels(channels), Format(format)
{
}

Image ReadImage(const std::filesystem::path& filepath, const bool flip)
{
    int width, height, channels = 0;
    const auto data =
        stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    const auto format = ParseImageFormat(channels);

    return Image(data, width, height, channels, format);
}

bool WriteImage(
    const std::filesystem::path& filepath, const Image& image, const bool flip)
{
    const auto fileExtension = filepath.extension();
    const auto fileFormat = ParseImageFileFormat(fileExtension.c_str());
    const auto writeResult = [filepath, image, fileFormat] {
        switch (fileFormat)
        {
        case ImageFileFormat::JPG:
            return stbi_write_jpg(filepath.c_str(),
                image.Width,
                image.Height,
                image.Channels,
                image.Buffer.data(),
                100);
        case ImageFileFormat::PNG:
            return stbi_write_png(filepath.c_str(),
                image.Width,
                image.Height,
                image.Channels,
                image.Buffer.data(),
                image.Width * image.Channels);
        case ImageFileFormat::BMP:
            return stbi_write_bmp(filepath.c_str(),
                image.Width,
                image.Height,
                image.Channels,
                image.Buffer.data());
        case ImageFileFormat::TGA:
            return stbi_write_tga(filepath.c_str(),
                image.Width,
                image.Height,
                image.Channels,
                image.Buffer.data());
        default:
            return 0;
        }
    }();

    return writeResult;
}

} // namespace Sennet
