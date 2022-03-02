#include "Pine/Network/Client.hpp"

#include <asio.hpp>

#include "Pine/Core/Base.hpp"

namespace Pine
{

bool IsConnected(const ClientState& client)
{
    return client.Connection ? IsConnected(*client.Connection.get()) : false;
}

bool Connect(ClientState& client, const std::string& host, const uint16_t port)
{
    if (client.ContextThread.joinable())
    {
        return false;
    }

    Resolver resolver(client.Context);
    SocketType socket(client.Context);
    const auto endpoints = resolver.resolve(host, std::to_string(port));

    client.Connection = std::make_unique<ConnectionState>(client.Context,
        std::move(socket),
        client.MessageQueue);
    ConnectToServer(*client.Connection.get(), endpoints);
    client.ContextThread = std::thread([&client]() { client.Context.run(); });
    return true;
}

void Disconnect(ClientState& client)
{
    if (IsConnected(client))
    {
        Disconnect(*client.Connection.get());
    }

    client.Context.stop();

    if (client.ContextThread.joinable())
    {
        client.ContextThread.join();
    }

    client.Connection.release();
}

void Send(const ClientState& client, const Message& message)
{
    if (IsConnected(client))
    {
        Send(*client.Connection.get(), message);
    }
}

} // namespace Pine
