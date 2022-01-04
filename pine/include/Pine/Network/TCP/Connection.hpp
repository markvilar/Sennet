#pragma once

#include <asio.hpp>

#include "Pine/Network/TCP/Message.hpp"
#include "Pine/Network/ThreadSafeQueue.hpp"

namespace Pine::TCP
{

template <typename T>
class Connection : public std::enable_shared_from_this<Connection<T>>
{
public:
    enum class Owner
    {
        Server,
        Client
    };

    Connection(Owner parent, asio::io_context& context,
        asio::ip::tcp::socket socket,
        ThreadSafeQueue<OwnedMessage<T>>& messagesIn)
        : m_Context(context), m_Socket(std::move(socket)),
          m_MessagesIn(messagesIn)
    {
        m_Owner = parent;
    }

    virtual ~Connection() {}

    uint32_t GetID() const { return m_ID; }

    void ConnectToClient(const uint32_t& id = 0)
    {
        if (m_Owner == Owner::Server)
        {
            if (m_Socket.is_open())
            {
                m_ID = id;
                ReadHeader();
            }
        }
    }

    void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
    {
        if (m_Owner == Owner::Client)
        {
            // TODO: Set up timeout.
            asio::async_connect(m_Socket,
                endpoints,
                [this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
                    if (!ec)
                    {
                        ReadHeader();
                    }
                });
        }
    }

    void Disconnect()
    {
        if (IsConnected())
        {
            asio::post(m_Context, [this]() { m_Socket.close(); });
        }
    }

    bool IsConnected() const { return m_Socket.is_open(); }

public:
    void Send(const Message<T>& message)
    {
        asio::post(m_Context, [this, message]() {
            bool writingMessages = !m_MessagesOut.empty();
            m_MessagesOut.push_back(message);
            if (!writingMessages)
            {
                WriteHeader();
            }
        });
    }

private:
    // ASYNC.
    void ReadHeader()
    {
        asio::async_read(m_Socket,
            asio::buffer(&m_MessageTemporaryIn.Header,
                sizeof(MessageHeader<T>)),
            [this](std::error_code ec, uint64_t length) {
                if (!ec)
                {
                    // If the message has a body.
                    if (m_MessageTemporaryIn.Header.Size > 0)
                    {
                        m_MessageTemporaryIn.Body.resize(
                            m_MessageTemporaryIn.Header.Size);
                        ReadBody();
                    }
                    else
                    {
                        AddToIncomingMessageQueue();
                    }
                }
                else
                {
                    PINE_CORE_ERROR("[{0}] Read Header Failed.", m_ID);
                    m_Socket.close();
                }
            });
    }

    // ASYNC.
    void ReadBody()
    {
        asio::async_read(m_Socket,
            asio::buffer(m_MessageTemporaryIn.Body.data(),
                m_MessageTemporaryIn.Body.size()),
            [this](std::error_code ec, uint64_t length) {
                if (!ec)
                {
                    AddToIncomingMessageQueue();
                }
                else
                {
                    PINE_CORE_ERROR("[{0}] ReadBody() failed.", m_ID);
                    m_Socket.close();
                }
            });
    }

    // ASYNC.
    void WriteHeader()
    {
        asio::async_write(m_Socket,
            asio::buffer(&m_MessagesOut.front().Header,
                sizeof(MessageHeader<T>)),
            [this](std::error_code ec, uint64_t length) {
                if (!ec)
                {
                    if (m_MessagesOut.front().Body.size() > 0)
                    {
                        WriteBody();
                    }
                    else
                    {
                        m_MessagesOut.pop_front();
                        if (!m_MessagesOut.empty())
                        {
                            WriteHeader();
                        }
                    }
                }
                else
                {
                    PINE_CORE_ERROR("[{0}] WriteHeader() failed.",
                        ec.message());
                    m_Socket.close();
                }
            });
    }

    // ASYNC.
    void WriteBody()
    {
        asio::async_write(m_Socket,
            asio::buffer(m_MessagesOut.front().Body.data(),
                m_MessagesOut.front().Body.size()),
            [this](std::error_code ec, uint64_t length) {
                if (!ec)
                {
                    m_MessagesOut.pop_front();

                    if (!m_MessagesOut.empty())
                    {
                        WriteHeader();
                    }
                }
                else
                {
                    PINE_CORE_ERROR("[{0}] WriteBody() failed.", m_ID);
                    m_Socket.close();
                }
            });
    }

    void AddToIncomingMessageQueue()
    {
        if (m_Owner == Owner::Server)
        {
            m_MessagesIn.push_back(
                {this->shared_from_this(), m_MessageTemporaryIn});
        }
        else
        {
            m_MessagesIn.push_back({nullptr, m_MessageTemporaryIn});
        }

        ReadHeader();
    }

protected:
    // Temporary.
    asio::ip::tcp::socket m_Socket;
    asio::io_context& m_Context;

    Owner m_Owner = Owner::Server;
    uint32_t m_ID = 0;

    // Outgoing message queue owned by connection.
    ThreadSafeQueue<Message<T>> m_MessagesOut;

    // Incoming message queue provided by client/server.
    ThreadSafeQueue<OwnedMessage<T>>& m_MessagesIn;

    // Message used to temporarily store incoming messages.
    Message<T> m_MessageTemporaryIn;
};

} // namespace Pine::TCP
