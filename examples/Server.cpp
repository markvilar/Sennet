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

class CustomServer : public Pine::TCP::Server<CustomMessageTypes>
{
public:
    CustomServer(const uint16_t port)
        : Pine::TCP::Server<CustomMessageTypes>(port)
    {
    }

protected:
    virtual bool OnClientConnect(
        std::shared_ptr<Pine::TCP::Connection<CustomMessageTypes>> client)
        override
    {
        return true;
    }

    virtual void OnClientDisconnect(
        std::shared_ptr<Pine::TCP::Connection<CustomMessageTypes>>) override
    {
    }

    virtual void OnMessage(
        std::shared_ptr<Pine::TCP::Connection<CustomMessageTypes>> client,
        Pine::TCP::Message<CustomMessageTypes>& message) override
    {
        switch (message.Header.ID)
        {
        case CustomMessageTypes::ServerPing:
            PINE_INFO("[{0}] Server Ping.", client->GetID());
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
    Pine::Log::Init();
    CustomServer server(60000);
    server.Start();

    while (true)
    {
        server.Update();
    }

    return 0;
}
