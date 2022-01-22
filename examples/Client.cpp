#include <chrono>
#include <functional>
#include <thread>

#include "Pine/Pine.hpp"
enum class CustomMessageTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

class CustomClient : public Pine::TCP::Client<CustomMessageTypes>
{
public:
    void PingServer()
    {
        Pine::TCP::Message<CustomMessageTypes> message;
        message.Header.ID = CustomMessageTypes::ServerPing;
        std::chrono::system_clock::time_point time =
            std::chrono::system_clock::now();

        message << time;
        Send(message);
    }
};

int main(int argc, char** argv)
{
    Pine::Log::Init();

    CustomClient client;
    client.Connect("127.0.0.1", 60000);

    bool quit = false;
    uint32_t pingCount = 0;
    const uint32_t totalPings = 10;
    while (!quit)
    {
        if (client.IsConnected())
        {
            if (pingCount < totalPings)
            {
                PINE_INFO("Pinging server!");
                client.PingServer();
                pingCount++;
            }

            if (!client.Incoming().empty())
            {
                auto message = client.Incoming().pop_front().Msg;
                switch (message.Header.ID)
                {
                case CustomMessageTypes::ServerPing:
                    std::chrono::system_clock::time_point sent;
                    auto received = std::chrono::system_clock::now();
                    message >> sent;
                    auto transmitTime =
                        std::chrono::duration<double>(received - sent).count();
                    PINE_INFO("Ping: {0}", transmitTime);
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            PINE_INFO("Server Down.");
            quit = true;
        }
    }

    return 0;
}
