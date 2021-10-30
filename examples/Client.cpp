#include <chrono>
#include <functional>
#include <thread>

#include "Sennet/Sennet.hpp"

enum class CustomMessageTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

class CustomClient : public Sennet::TCP::Client<CustomMessageTypes>
{
public:
    void PingServer()
    {
        Sennet::TCP::Message<CustomMessageTypes> message;
        message.Header.ID = CustomMessageTypes::ServerPing;
        std::chrono::system_clock::time_point time =
            std::chrono::system_clock::now();

        message << time;
        Send(message);
    }
};

int main(int argc, char** argv)
{
    Sennet::Log::Init();

    CustomClient client;
    client.Connect("10.42.0.35", 60000);

    bool quit = false;
    bool sent = false;
    while (!quit)
    {
        if (client.IsConnected())
        {
            if (!sent)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                SENNET_INFO("Pinging server!");
                client.PingServer();
                sent = true;
            }
            if (!client.Incoming().empty())
            {
                auto message = client.Incoming().pop_front().Msg;
                std::chrono::system_clock::time_point t;
                std::chrono::system_clock::time_point z;
                double time;

                switch (message.Header.ID)
                {
                case CustomMessageTypes::ServerPing:
                    t = std::chrono::system_clock::now();
                    message >> z;
                    time = std::chrono::duration<double>(t - z).count();
                    SENNET_INFO("Ping: {0}", time);
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            SENNET_INFO("Server Down.");
            quit = true;
        }
    }

    return 0;
}
