#include "Sennet/Sennet.hpp"

int main(int argc, char** argv)
{
    Sennet::Log::Init();
    auto image = Sennet::ReadImage("../resources/images/Plant-HD2K.png");
    auto jpg = Sennet::WriteImage("./test.jpg", image);
    auto png = Sennet::WriteImage("./test.png", image);
    auto bmp = Sennet::WriteImage("./test.bmp", image);
    auto tga = Sennet::WriteImage("./test.tga", image);

    SENNET_INFO("JPG: {}, PNG: {}, BMP: {}, TGA: {}", jpg, png, bmp, tga);
}
