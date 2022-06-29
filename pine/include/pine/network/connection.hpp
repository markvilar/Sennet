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
    ConnectionState(NetworkContext& context, SocketType socket,
        LockedQueue<std::vector<uint8_t>>& owner_queue)
        : context(context), socket(std::move(socket)), read_queue(owner_queue)
    {
    }
};

bool IsConnected(const ConnectionState& connection);
void Disconnect(ConnectionState& connection);

void ConnectToClient(ConnectionState& connection);
void ConnectToServer(ConnectionState& connection, const ResolveType& endpoints);

void ReadMessageSize(ConnectionState& connection);
void ReadMessage(ConnectionState& connection, const uint64_t size);

void Send(ConnectionState& connection, const uint8_t* data,
    const uint64_t size);

void WriteMessageSize(ConnectionState& connection);
void WriteMessage(ConnectionState& connection);

}; // namespace pine
