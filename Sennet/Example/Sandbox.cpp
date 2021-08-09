#include "Sennet/Sennet.hpp"

int main(int argc, char** argv)
{
    Sennet::Log::Init();

    Sennet::Socket::Specification spec;

    auto socket = Sennet::Socket::Create(spec);
    socket->Bind("127.0.0.1", 8000);
    socket->Listen();
    socket->Accept("198.0.0.1", 9000);
}
