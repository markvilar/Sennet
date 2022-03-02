#pragma once

#include <memory>

#include "Pine/Core/Base.hpp"
#include "Pine/Network/Connection.hpp"
#include "Pine/Network/Message.hpp"
#include "Pine/Network/Types.hpp"

namespace Pine
{

struct ServerState
{
    // TODO: Replace with unique pointer.
    using Connection = ConnectionState;
    using ConnectionPtr = std::shared_ptr<ConnectionState>;

    NetworkContext Context{};
    std::thread ContextThread{};
    AcceptorType Acceptor;

    std::deque<ConnectionPtr> Connections{};
    ThreadSafeQueue<Message> MessageQueue{};

public:
    ServerState(const uint16_t port);

    ServerState(const ServerState&) = delete;
    ServerState(ServerState&&) = delete;

    ServerState& operator=(const ServerState&) = delete;
    ServerState& operator=(ServerState&&) = delete;

    ~ServerState();
};

void StopServer(ServerState& server);
void SendToClient(ServerState& server,
    const std::shared_ptr<ConnectionState>& client, const Message& message);

template <typename ConnectHandler>
void ListenForClients(ServerState& server, ConnectHandler& handler)
{
    server.Acceptor.async_accept(
        [&server, &handler](const std::error_code ec, SocketType socket) {
            if (!ec)
            {
                auto client = std::make_shared<ConnectionState>(server.Context,
                    std::move(socket),
                    server.MessageQueue);

                if (handler(*client.get()))
                {
                    server.Connections.push_back(client);
                    ConnectToClient(*client.get());
                }
            }
            else
            {
                PINE_CORE_ERROR("Server error: {0}", ec.message());
            }

            ListenForClients(server, handler);
        });
}

template <typename ConnectHandler>
bool StartServer(ServerState& server, ConnectHandler handler)
{
    try
    {
        ListenForClients(server, handler);
        server.ContextThread =
            std::thread([&server]() { server.Context.run(); });
    }
    catch (const std::exception& error)
    {
        PINE_CORE_ERROR("Server exception: {0}", error.what());
        return false;
    }
    return true;
}

template <typename MessageCallback>
void UpdateServer(ServerState& server, MessageCallback callback,
    const uint64_t maxMessages = -1)
{
    uint64_t messageCount = 0;
    while (messageCount < maxMessages && !server.MessageQueue.empty())
    {
        const auto message = server.MessageQueue.pop_front();
        callback(message);
        messageCount++;
    }
}

}; // namespace Pine
