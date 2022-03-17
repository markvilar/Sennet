#pragma once

#include <memory>

#include "Pine/Network/Message.hpp"
#include "Pine/Network/Types.hpp"
#include "Pine/Utils/ThreadSafeQueue.hpp"

namespace Pine
{

struct ConnectionState
{
    SocketType Socket;
    ThreadSafeQueue<Message> WriteQueue{};

    NetworkContext& Context;
    ThreadSafeQueue<Message>& ReadQueue;

public:
    ConnectionState(NetworkContext& context, SocketType socket,
        ThreadSafeQueue<Message>& readQueue)
        : Context(context), Socket(std::move(socket)), ReadQueue(readQueue)
    {
    }
};

bool IsConnected(const ConnectionState& connection);
void Disconnect(ConnectionState& connection);

void ConnectToClient(ConnectionState& connection);
void ConnectToServer(ConnectionState& connection, const ResolveType& endpoints);

void ReadMessageHeader(ConnectionState& connection);
void ReadMessageBody(
    ConnectionState& connection, const std::shared_ptr<Message>& message);

void Send(ConnectionState& connection, const Message& message);

void WriteMessageHeader(ConnectionState& connection);
void WriteMessageBody(ConnectionState& connection);

}; // namespace Pine
