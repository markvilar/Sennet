#include "pine/network/client.hpp"

#include <asio.hpp>

namespace pine {

bool is_connected(const ClientState& client) {
    return client.connection ? is_connected(*client.connection.get()) : false;
}

bool connect(ClientState& client,
    const std::string& host,
    const uint16_t port) {
    if (client.context_thread.joinable()) {
        return false;
    }

    Resolver resolver(client.context);
    SocketType socket(client.context);
    const auto endpoints = resolver.resolve(host, std::to_string(port));

    client.connection = std::make_unique<ConnectionState>(client.context,
        std::move(socket),
        client.message_queue);
    connect_to_server(*client.connection.get(), endpoints);
    client.context_thread = std::thread([&client]() {
        client.context.restart();
        client.context.run();
    });
    return true;
}

void disconnect(ClientState& client) {
    if (is_connected(client)) {
        disconnect(*client.connection.get());
    }

    client.context.stop();

    if (client.context_thread.joinable()) {
        client.context_thread.join();
    }

    client.connection.release();
}

void send(const ClientState& client, const uint8_t* data, const uint64_t size) {
    if (is_connected(client)) {
        send(*client.connection.get(), data, size);
    }
}

} // namespace pine
