#include "Pine/Network/Connection.hpp"

#include <asio.hpp>

#include "Pine/Core/Base.hpp"

namespace Pine
{

bool IsConnected(const ConnectionState& connection)
{
    return connection.socket.is_open();
}

void Disconnect(ConnectionState& connection)
{
    if (IsConnected(connection))
    {
        asio::post(connection.context,
            [&connection]() { connection.socket.close(); });
    }
}

void ConnectToClient(ConnectionState& connection)
{
    if (IsConnected(connection))
    {
        ReadMessageSize(connection);
    }
}

void ConnectToServer(ConnectionState& connection, const ResolveType& endpoints)
{
    // TODO: Set up timeout.
    asio::async_connect(connection.socket,
        endpoints,
        [&connection](const std::error_code error, const EndpointType endpoint)
        {
            if (error)
            {
                PINE_CORE_ERROR("Connection to server failed: {0}",
                    error.message());
                return;
            }
            ReadMessageSize(connection);
        });
}

void ReadMessageSize(ConnectionState& connection)
{
    // FIXME: Segmentation fault
    auto message_size = std::make_shared<uint64_t>(0);

    // TODO: Look into solution for copying message_size into asio, instead
    // of making it shared.
    asio::async_read(connection.socket,
        asio::mutable_buffer(&*message_size.get(), sizeof(*message_size.get())),
        [&connection, message_size](const std::error_code error,
            const uint64_t length)
        {
            if (error)
            {
                PINE_CORE_ERROR("Read size error: {0}", error.message());
                connection.socket.close();
                return;
            }

            if (message_size > 0)
            {
                ReadMessage(connection, *message_size.get());
            }
            else
            {
                ReadMessageSize(connection);
            }
        });
}

void ReadMessage(ConnectionState& connection, const uint64_t size)
{

    auto message = std::make_shared<std::vector<uint8_t>>();
    message->resize(size);

    asio::async_read(connection.socket,
        asio::mutable_buffer(message->data(), message->size()),
        [&connection, message](const std::error_code error,
            const uint64_t length)
        {
            if (error)
            {
                PINE_CORE_ERROR("Read message: {0}", error.message());
                connection.socket.close();
                return;
            }

            connection.read_queue.push_back(std::move(*message.get()));
            ReadMessageSize(connection);
        });
}

void Send(ConnectionState& connection, const uint8_t* data, const uint64_t size)
{
    std::vector<uint8_t> buffer(data, data + size);

    asio::post(connection.context,
        [&connection, buffer]() -> void
        {
            const auto is_writing = !connection.write_queue.empty();
            connection.write_queue.push_back(std::move(buffer));
            if (!is_writing)
            {
                WriteMessageSize(connection);
            }
        });
}

void WriteMessageSize(ConnectionState& connection)
{
    auto message_size =
        std::make_shared<uint64_t>(connection.write_queue.front().size());

    // TODO: Look at possibility to move the size into asio, and not use a
    // shared pointer.
    asio::async_write(connection.socket,
        asio::const_buffer(&*message_size.get(), sizeof(*message_size.get())),
        [&connection, message_size](const std::error_code error,
            const uint64_t length) -> void
        {
            if (error)
            {
                PINE_CORE_ERROR("Write message size: {0}", error.message());
                connection.socket.close();
                return;
            }

            if (*message_size.get() > 0)
            {
                WriteMessage(connection);
            }
            else
            {
                connection.write_queue.pop_front();
                if (!connection.write_queue.empty())
                {
                    WriteMessageSize(connection);
                }
            }
        });
}

void WriteMessage(ConnectionState& connection)
{
    asio::async_write(connection.socket,
        asio::const_buffer(connection.write_queue.front().data(),
            connection.write_queue.front().size()),
        [&connection](const std::error_code error,
            const uint64_t length) -> void
        {
            if (error)
            {
                PINE_CORE_ERROR("Write message: {0}", error.message());
                connection.socket.close();
                return;
            }

            connection.write_queue.pop_front();
            if (!connection.write_queue.empty())
            {
                WriteMessageSize(connection);
            }
        });
}

} // namespace Pine
