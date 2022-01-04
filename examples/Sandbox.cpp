#include "Pine/Pine.hpp"

int main(int argc, char** argv)
{
    Pine::Log::Init();
    auto image = Pine::ReadImage("../resources/images/Plant-HD2K.png");
    auto jpg = Pine::WriteImage("./test.jpg", image);
    auto png = Pine::WriteImage("./test.png", image);
    auto bmp = Pine::WriteImage("./test.bmp", image);
    auto tga = Pine::WriteImage("./test.tga", image);

    PINE_INFO("JPG: {}, PNG: {}, BMP: {}, TGA: {}", jpg, png, bmp, tga);
}
