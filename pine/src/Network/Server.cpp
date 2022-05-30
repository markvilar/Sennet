#include "Pine/Network/Server.hpp"

#include <asio.hpp>

namespace Pine
{

ServerState::ServerState(const uint16_t port)
    : acceptor(context, EndpointType(asio::ip::tcp::v4(), port))
{
}

ServerState::~ServerState()
{
    context.stop();
    if (context_thread.joinable())
    {
        context_thread.join();
    }
}

void StopServer(ServerState& server)
{
    server.context.stop();
    if (server.context_thread.joinable())
    {
        server.context_thread.join();
    }
}

void SendToClient(ServerState& server,
    const std::shared_ptr<ConnectionState>& client, const uint8_t* data,
    const uint64_t size)
{
    if (client)
    {
        if (IsConnected(*client.get()))
        {
            Send(*client.get(), data, size);
        }
    }
    else
    {
        server.connections.erase(std::remove(server.connections.begin(),
                                     server.connections.end(),
                                     client),
            server.connections.end());
    }
}

}; // namespace Pine
