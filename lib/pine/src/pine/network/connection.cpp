#include "pine/network/connection.hpp"

#include <asio.hpp>

#include "pine/core/log.hpp"

namespace pine {

bool is_connected(const ConnectionState& connection) {
    return connection.socket.is_open();
}

void disconnect(ConnectionState& connection) {
    if (is_connected(connection)) {
        asio::post(connection.context,
            [&connection]() { connection.socket.close(); });
    }
}

void connect_to_client(ConnectionState& connection) {
    if (is_connected(connection)) {
        read_message_size(connection);
    }
}

void connect_to_server(ConnectionState& connection,
    const ResolveType& endpoints) {
    asio::async_connect(connection.socket,
        endpoints,
        [&connection](const std::error_code error,
            [[maybe_unused]] const EndpointType endpoint) {
            if (error) {
                PINE_CORE_ERROR("Connection to server failed: {0}",
                    error.message());
                return;
            }
            read_message_size(connection);
        });
}

void read_message_size(ConnectionState& connection) {
    auto message_size = std::make_shared<uint64_t>(0);

    asio::async_read(connection.socket,
        asio::mutable_buffer(&*message_size.get(), sizeof(*message_size.get())),
        [&connection, message_size](const std::error_code error,
            [[maybe_unused]] const uint64_t length) {
            if (error) {
                PINE_CORE_ERROR("Read size error: {0}", error.message());
                return;
            }

            if (message_size > 0) {
                read_message(connection, *message_size.get());
            } else {
                read_message_size(connection);
            }
        });
}

void read_message(ConnectionState& connection, const uint64_t size) {
    auto message = std::make_shared<std::vector<uint8_t>>();
    message->resize(size);

    asio::async_read(connection.socket,
        asio::mutable_buffer(message->data(), message->size()),
        [&connection, message](const std::error_code error,
            [[maybe_unused]] const uint64_t length) {
            if (error) {
                PINE_CORE_ERROR("Read message: {0}", error.message());
                return;
            }

            connection.read_queue.push_back(std::move(*message.get()));
            read_message_size(connection);
        });
}

void send(ConnectionState& connection,
    const uint8_t* data,
    const uint64_t size) {
    std::vector<uint8_t> buffer(data, data + size);

    asio::post(connection.context, [&connection, buffer]() -> void {
        const auto is_writing = !connection.write_queue.empty();
        connection.write_queue.push_back(std::move(buffer));
        if (!is_writing) {
            write_message_size(connection);
        }
    });
}

void write_message_size(ConnectionState& connection) {
    auto message_size =
        std::make_shared<uint64_t>(connection.write_queue.front().size());

    // TODO: Look at possibility to move the size into asio, and not use a
    // shared pointer.
    asio::async_write(connection.socket,
        asio::const_buffer(&*message_size.get(), sizeof(*message_size.get())),
        [&connection, message_size](const std::error_code error,
            [[maybe_unused]] const uint64_t length) -> void {
            if (error) {
                PINE_CORE_ERROR("Write message size: {0}", error.message());
                return;
            }

            if (*message_size.get() > 0) {
                write_message(connection);
            } else {
                connection.write_queue.pop_front();
                if (!connection.write_queue.empty()) {
                    write_message_size(connection);
                }
            }
        });
}

void write_message(ConnectionState& connection) {
    asio::async_write(connection.socket,
        asio::const_buffer(connection.write_queue.front().data(),
            connection.write_queue.front().size()),
        [&connection](const std::error_code error,
            [[maybe_unused]] const uint64_t length) -> void {
            if (error) {
                PINE_CORE_ERROR("Write message: {0}", error.message());
                return;
            }

            connection.write_queue.pop_front();
            if (!connection.write_queue.empty()) {
                write_message_size(connection);
            }
        });
}

} // namespace pine
