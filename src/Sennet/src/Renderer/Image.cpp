#include "Sennet/Renderer/Image.hpp"
#include "Sennet/Pch.hpp"

#define STBI_NO_SIMD
#define STBI_NO_GIF
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

namespace Sennet
{

constexpr ImageFormat ParseImageFormat(const uint8_t channels)
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
        return ImageFormat::GRAY;
    }
}

constexpr ImageFileFormat ParseImageFileFormat(const std::string_view fileExtension)
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

constexpr uint32_t NumberOfChannels(const ImageFormat format)
{
    switch (format)
    {
    case ImageFormat::UNKNOWN:    return 0;
    case ImageFormat::GRAY:       return 1;
    case ImageFormat::GRAY_ALPHA: return 2;
    case ImageFormat::RGB:        return 3;
    case ImageFormat::BGR:        return 3;
    case ImageFormat::RGBA:       return 4;
    case ImageFormat::BGRA:       return 4;
    default:                      return 0;
    }
}

Image::Image(const uint8_t* data, const uint32_t width, const uint32_t height, 
    const ImageFormat format)
    : Width(width), Height(height), Format(format)
{
    const auto channels = NumberOfChannels(format);
    Buffer = std::vector<uint8_t>(data, data + width * height * channels);
}

Image ReadImage(const std::filesystem::path& filepath, const bool flip)
{
    int width, height, channels = 0;

    stbi_set_flip_vertically_on_load(flip);

    const auto data =
        stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    const auto format = ParseImageFormat(channels);

    return Image(data, width, height, format);
}

Image ReadImage(const std::filesystem::path& filepath, const ImageFormat format, const bool flip)
{
    int width, height, channels = 0;
    auto desiredChannels = static_cast<int>(NumberOfChannels(format));

    stbi_set_flip_vertically_on_load(flip);

    const auto data =
        stbi_load(filepath.c_str(), &width, &height, &channels, 
        desiredChannels);

    return Image(data, width, height, format);
}

bool WriteImage(
    const std::filesystem::path& filepath, const Image& image, const bool flip)
{
    const auto fileExtension = filepath.extension();
    const auto fileFormat = ParseImageFileFormat(fileExtension.c_str());

    stbi_set_flip_vertically_on_load(flip);

    const auto channels = NumberOfChannels(image.Format);

    const auto writeResult = [filepath, image, channels, fileFormat](){
        switch (fileFormat)
        {
        case ImageFileFormat::JPG:
            return stbi_write_jpg(filepath.c_str(),
                image.Width,
                image.Height,
                channels,
                image.Buffer.data(),
                100);
        case ImageFileFormat::PNG:
            return stbi_write_png(filepath.c_str(),
                image.Width,
                image.Height,
                channels,
                image.Buffer.data(),
                image.Width * channels);
        case ImageFileFormat::BMP:
            return stbi_write_bmp(filepath.c_str(),
                image.Width,
                image.Height,
                channels,
                image.Buffer.data());
        case ImageFileFormat::TGA:
            return stbi_write_tga(filepath.c_str(),
                image.Width,
                image.Height,
                channels,
                image.Buffer.data());
        default:
            return 0;
        }
    }();

    return writeResult;
}

} // namespace Sennet
