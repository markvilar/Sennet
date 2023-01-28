#pragma once

#include <cstdint>
#include <memory>
#include <thread>

#include "pine/network/connection.hpp"
#include "pine/network/types.hpp"

namespace pine
{
struct ClientState
{
    NetworkContext context{};
    std::thread context_thread{};

    std::unique_ptr<ConnectionState> connection{};
    LockedQueue<std::vector<uint8_t>> message_queue{};

public:
    ClientState() = default;

    ClientState(const ClientState&) = delete;
    ClientState(ClientState&&) = delete;

    ClientState& operator=(const ClientState&) = delete;
    ClientState& operator=(ClientState&&) = delete;

    ~ClientState()
    {
        context.stop();
        if (context_thread.joinable())
        {
            context_thread.join();
        }
    }
};

bool is_connected(const ClientState& client);
bool connect(ClientState& client, const std::string& host, const uint16_t port);
void disconnect(ClientState& client);

void send(const ClientState& client, const uint8_t* data, const uint64_t size);

} // namespace pine
