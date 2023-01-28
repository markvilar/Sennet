#pragma once

#include <cstdint>
#include <memory>

#include "pine/network/types.hpp"
#include "pine/utils/locked_queue.hpp"

namespace pine
{
struct ConnectionState
{
    using size_t = uint64_t;

    SocketType socket;
    LockedQueue<std::vector<uint8_t>> write_queue{};

    NetworkContext& context;
    LockedQueue<std::vector<uint8_t>>& read_queue;

public:
    ConnectionState(NetworkContext& context,
        SocketType socket,
        LockedQueue<std::vector<uint8_t>>& owner_queue)
        : socket(std::move(socket)),
          context(context),
          read_queue(owner_queue)
    {
    }

    ~ConnectionState() { socket.close(); }
};

bool is_connected(const ConnectionState& connection);
void disconnect(ConnectionState& connection);

void connect_to_client(ConnectionState& connection);
void connect_to_server(ConnectionState& connection,
    const ResolveType& endpoints);

void read_message_size(ConnectionState& connection);
void read_message(ConnectionState& connection, const uint64_t size);

void send(ConnectionState& connection,
    const uint8_t* data,
    const uint64_t size);

void write_message_size(ConnectionState& connection);
void write_message(ConnectionState& connection);

} // namespace pine
