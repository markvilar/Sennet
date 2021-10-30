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

class CustomServer : public Sennet::TCP::Server<CustomMessageTypes>
{
public:
    CustomServer(uint16_t port) : Sennet::TCP::Server<CustomMessageTypes>(port)
    {
    }

protected:
    virtual bool OnClientConnect(
        Sennet::Ref<Sennet::TCP::Connection<CustomMessageTypes>> client)
        override
    {
        return true;
    }

    virtual void OnClientDisconnect(
        Sennet::Ref<Sennet::TCP::Connection<CustomMessageTypes>>) override
    {
    }

    virtual void OnMessage(
        Sennet::Ref<Sennet::TCP::Connection<CustomMessageTypes>> client,
        Sennet::TCP::Message<CustomMessageTypes>& message) override
    {
        switch (message.Header.ID)
        {
        case CustomMessageTypes::ServerPing:
            SENNET_INFO("[{0}] Server Ping.", client->GetID());
            // Bounce message back to client.
            client->Send(message);
            break;
        default:
            break;
        }
    }
};

int main(int argc, char** argv)
{
    Sennet::Log::Init();
    CustomServer server(60000);
    server.Start();

    while (true)
    {
        server.Update();
    }

    return 0;
}
