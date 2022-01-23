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

class CustomServer : public Pine::TCPServer<CustomMessageTypes>
{
public:
    CustomServer(const uint16_t port)
        : Pine::TCPServer<CustomMessageTypes>(port)
    {
    }

protected:
    virtual bool OnClientConnect(
        std::shared_ptr<Pine::Connection<CustomMessageTypes>> client) override
    {
        return true;
    }

    virtual void OnClientDisconnect(
        std::shared_ptr<Pine::Connection<CustomMessageTypes>>) override
    {
    }

    virtual void OnMessage(
        std::shared_ptr<Pine::Connection<CustomMessageTypes>> client,
        Pine::Message<CustomMessageTypes>& message) override
    {
        switch (message.Header.ID)
        {
        case CustomMessageTypes::ServerPing:
            PINE_INFO("[{0}] Server Ping.", client->GetID());
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
