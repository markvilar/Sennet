#include <iostream>
#include <string>

#include <asio.hpp>

class SerialService
{
public:
    SerialService(asio::io_context& context)
        : port(context), timer(context)
    {
    }

    void open(const std::string& device, const int rate)
    {
        try
        {
            port.open(device);
            port.set_option(asio::serial_port::baud_rate(rate));
        }
        catch (const std::system_error& error)
        {
            std::cout << "Error when opening " << device << ": " << error.what() << "\n";
        }
    }

private:
    asio::serial_port port;
    asio::steady_timer timer;
    asio::streambuf buffer;
};

int main(const int argc, const char** argv)
{
    asio::io_context context;
    SerialService service(context);
  
    const auto device = argc > 1 ? argv[1] : "/dev/i2c-1";
    static constexpr auto rate = 9600;
    std::cout << device << "\n";
    service.open(device, rate);

    return 0;
}
