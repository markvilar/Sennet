#include "pine/network/server.hpp"

#include <asio.hpp>

namespace pine
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

void stop_server(ServerState& server)
{
    server.context.stop();
    if (server.context_thread.joinable())
    {
        server.context_thread.join();
    }
}

void send_to_client(ServerState& server,
    const std::shared_ptr<ConnectionState>& client, const uint8_t* data,
    const uint64_t size)
{
    if (client)
    {
        if (is_connected(*client.get()))
        {
            send(*client.get(), data, size);
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

}; // namespace pine
