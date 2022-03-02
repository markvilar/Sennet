#pragma once

#include <memory>
#include <thread>

#include "Pine/Network/Connection.hpp"
#include "Pine/Network/Message.hpp"
#include "Pine/Network/Types.hpp"

namespace Pine
{

struct ClientState
{
    NetworkContext Context{};
    std::thread ContextThread{};

    std::unique_ptr<ConnectionState> Connection{};
    ThreadSafeQueue<Message> MessageQueue{};

public:
    ClientState() = default;

    ClientState(const ClientState&) = delete;
    ClientState(ClientState&&) = delete;

    ClientState& operator=(const ClientState&) = delete;
    ClientState& operator=(ClientState&&) = delete;

    ~ClientState()
    {
        Context.stop();
        if (ContextThread.joinable())
        {
            ContextThread.join();
        }
    }
};

bool IsConnected(const ClientState& client);
bool Connect(ClientState& client, const std::string& host, const uint16_t port);
void Disconnect(ClientState& client);

void Send(const ClientState& client, const Message& message);

}; // namespace Pine
