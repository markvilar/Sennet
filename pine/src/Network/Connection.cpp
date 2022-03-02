#include "Pine/Network/Connection.hpp"

#include <asio.hpp>

#include "Pine/Core/Base.hpp"

namespace Pine
{

bool IsConnected(const ConnectionState& connection)
{
    return connection.Socket.is_open();
}

void Disconnect(ConnectionState& connection)
{
    if (IsConnected(connection))
    {
        asio::post(connection.Context,
            [&connection]() { connection.Socket.close(); });
    }
}

void ConnectToClient(ConnectionState& connection)
{
    if (IsConnected(connection))
    {
        ReadMessageHeader(connection);
    }
}

void ConnectToServer(ConnectionState& connection, const ResolveType& endpoints)
{
    // TODO: Set up timeout.
    asio::async_connect(connection.Socket,
        endpoints,
        [&connection](const std::error_code error,
            const EndpointType endpoint) {
            if (error)
            {
                PINE_CORE_ERROR("Connection to server failed: {0}",
                    error.message());
                return;
            }
            ReadMessageHeader(connection);
        });
}

void ReadMessageHeader(ConnectionState& connection)
{
    auto message = std::make_shared<Message>();

    asio::async_read(connection.Socket,
        asio::buffer(&message->Header, sizeof(MessageHeader)),
        [&connection, message](const std::error_code error,
            const uint64_t length) {
            if (error)
            {
                PINE_CORE_ERROR("Read header failed: {0}", error.message());
                connection.Socket.close();
                return;
            }

            if (message->Header.Size > 0)
            {
                message->Body.resize(message->Header.Size);
                ReadMessageBody(connection, message);
            }
            else
            {
                ReadMessageHeader(connection);
            }
        });
}

void ReadMessageBody(
    ConnectionState& connection, const std::shared_ptr<Message>& message)
{
    asio::async_read(connection.Socket,
        asio::buffer(message->Body.data(), message->Body.size()),
        [&connection, message](const std::error_code error,
            const uint64_t length) {
            if (error)
            {
                PINE_CORE_ERROR("Read body failed: {0}", error.message());
                connection.Socket.close();
                return;
            }

            connection.ReadQueue.push_back(std::move(*message.get()));
            ReadMessageHeader(connection);
        });
}

void Send(ConnectionState& connection, const Message& message)
{
    asio::post(connection.Context, [&connection, message]() {
        const auto isWriting = !connection.WriteQueue.empty();
        connection.WriteQueue.push_back(std::move(message));
        if (!isWriting)
        {
            WriteMessageHeader(connection);
        }
    });
}

void WriteMessageHeader(ConnectionState& connection)
{
    asio::async_write(connection.Socket,
        asio::buffer(&connection.WriteQueue.front().Header,
            sizeof(MessageHeader)),
        [&connection](const std::error_code error, const uint64_t length) {
            if (error)
            {
                PINE_CORE_ERROR("Write header failed: {0}", error.message());
                connection.Socket.close();
                return;
            }

            if (connection.WriteQueue.front().Body.size() > 0)
            {
                WriteMessageBody(connection);
            }
            else
            {
                connection.WriteQueue.pop_front();
                if (!connection.WriteQueue.empty())
                {
                    WriteMessageHeader(connection);
                }
            }
        });
}

void WriteMessageBody(ConnectionState& connection)
{
    asio::async_write(connection.Socket,
        asio::buffer(connection.WriteQueue.front().Body.data(),
            connection.WriteQueue.front().Body.size()),
        [&connection](const std::error_code error, const uint64_t length) {
            if (error)
            {
                PINE_CORE_ERROR("Write body failed: {0}", error.message());
                connection.Socket.close();
                return;
            }

            connection.WriteQueue.pop_front();
            if (!connection.WriteQueue.empty())
            {
                WriteMessageHeader(connection);
            }
        });
}

} // namespace Pine
