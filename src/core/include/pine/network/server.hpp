#pragma once

#include <functional>
#include <limits>
#include <memory>

#include "pine/core/common.hpp"
#include "pine/network/connection.hpp"
#include "pine/network/types.hpp"
#include "pine/utils/locked_queue.hpp"

namespace pine
{

struct ServerState
{
    NetworkContext context{};
    std::thread context_thread{};
    AcceptorType acceptor;

    std::deque<std::shared_ptr<ConnectionState>> connections{};
    LockedQueue<std::vector<uint8_t>> message_queue{};

    // Callbacks
    std::function<bool(const ConnectionState&)> connection_callback =
        [](const ConnectionState&){ return false; };
    std::function<void(const std::vector<uint8_t>&)> message_callback =
        [](const std::vector<uint8_t>&){};

public:
    ServerState(const uint16_t port);

    ServerState(const ServerState&) = delete;
    ServerState(ServerState&&) = delete;

    ServerState& operator=(const ServerState&) = delete;
    ServerState& operator=(ServerState&&) = delete;

    ~ServerState();

    template <typename Callable>
    void set_connection_callback(const Callable& callable)
    {
        connection_callback = callable;
    }

    template <typename Callable>
    void set_message_callback(const Callable& callable)
    {
        message_callback = callable;
    }
};

void stop_server(ServerState& server);
void send_to_client(ServerState& server,
    const std::shared_ptr<ConnectionState>& client, 
    const uint8_t* data,
    const uint64_t size);

inline void listen_for_clients(ServerState& server)
{
    server.acceptor.async_accept(
        [&server](const std::error_code ec, SocketType socket)
        {
            if (!ec)
            {
                auto client = std::make_shared<ConnectionState>(server.context,
                    std::move(socket),
                    server.message_queue);

                if (server.connection_callback(*client.get()))
                {
                    server.connections.push_back(client);
                    connect_to_client(*client.get());
                }
            }
            else
            {
                PINE_CORE_ERROR("Server error: {0}", ec.message());
            }

            listen_for_clients(server);
        });
}

inline bool start_server(ServerState& server)
{
    try
    {
        listen_for_clients(server);
        server.context_thread = std::thread([&server]() { server.context.run(); });
    }
    catch (const std::exception& error)
    {
        PINE_CORE_ERROR("Server exception: {0}", error.what());
        return false;
    }
    return true;
}

inline void update_server(ServerState& server, 
    const uint64_t max_messages = std::numeric_limits<uint64_t>::max())
{
    uint64_t message_count = 0;
    while (message_count < max_messages && !server.message_queue.empty())
    {
        const auto message = server.message_queue.pop_front();
        server.message_callback(message);
        message_count++;
    }
}

} // namespace pine
