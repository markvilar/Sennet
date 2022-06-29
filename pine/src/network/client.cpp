#include "pine/network/client.hpp"

#include <asio.hpp>

#include "pine/core/common.hpp"

namespace pine
{

bool IsConnected(const ClientState& client)
{
    return client.connection ? IsConnected(*client.connection.get()) : false;
}

bool Connect(ClientState& client, const std::string& host, const uint16_t port)
{
    if (client.context_thread.joinable())
    {
        return false;
    }

    Resolver resolver(client.context);
    SocketType socket(client.context);
    const auto endpoints = resolver.resolve(host, std::to_string(port));

    client.connection = std::make_unique<ConnectionState>(client.context,
        std::move(socket),
        client.message_queue);
    ConnectToServer(*client.connection.get(), endpoints);
    client.context_thread = std::thread([&client]() { client.context.run(); });
    return true;
}

void Disconnect(ClientState& client)
{
    if (IsConnected(client))
    {
        Disconnect(*client.connection.get());
    }

    client.context.stop();

    if (client.context_thread.joinable())
    {
        client.context_thread.join();
    }

    client.connection.release();
}

void Send(const ClientState& client, const uint8_t* data, const uint64_t size)
{
    if (IsConnected(client))
    {
        Send(*client.connection.get(), data, size);
    }
}

} // namespace pine
