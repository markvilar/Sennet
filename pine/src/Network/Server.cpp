#include "Pine/Network/Server.hpp"

#include <asio.hpp>

namespace Pine
{

ServerState::ServerState(const uint16_t port)
    : Acceptor(Context, EndpointType(asio::ip::tcp::v4(), port))
{
}

ServerState::~ServerState()
{
    Context.stop();
    if (ContextThread.joinable())
    {
        ContextThread.join();
    }
}

void StopServer(ServerState& server)
{
    server.Context.stop();
    if (server.ContextThread.joinable())
    {
        server.ContextThread.join();
    }
}

void SendToClient(ServerState& server,
    const std::shared_ptr<ConnectionState>& client, const Message& message)
{
    if (client)
    {
        if (IsConnected(*client.get()))
        {
            Send(*client.get(), message);
        }
    }
    else
    {
        server.Connections.erase(std::remove(server.Connections.begin(),
                                     server.Connections.end(),
                                     client),
            server.Connections.end());
    }
}

}; // namespace Pine
