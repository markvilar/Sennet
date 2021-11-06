#pragma once

#include <asio.hpp>

#include "Sennet/Network/TCP/Connection.hpp"
#include "Sennet/Network/TCP/Message.hpp"
#include "Sennet/Network/ThreadSafeQueue.hpp"

namespace Sennet
{

namespace TCP
{

template <typename T> class Server
{
    // Server class interface.
public:
    Server(uint16_t port)
        : m_Acceptor(
              m_Context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
    }

    virtual ~Server() { Stop(); }

    bool Start()
    {
        try
        {
            WaitForClientConnection();

            m_ContextThread = std::thread([this]() { m_Context.run(); });
        }
        catch (std::exception& e)
        {
            SENNET_CORE_ERROR("[Server] Exception: {0}", e.what());
            return false;
        }

        SENNET_CORE_INFO("[Server] Started!");
        return true;
    }

    void Stop()
    {
        m_Context.stop();

        if (m_ContextThread.joinable())
        {
            m_ContextThread.join();
        }

        SENNET_CORE_INFO("[Server] Stopped!");
    }

    // ASYNC - Instruct asio to wait for connection.
    void WaitForClientConnection()
    {
        m_Acceptor.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket) {
                if (!ec)
                {
                    SENNET_CORE_INFO("[Server] New Connection: {0}",
                        socket.remote_endpoint());

                    auto newConn =
                        CreateRef<Connection<T>>(Connection<T>::Owner::Server,
                            m_Context,
                            std::move(socket),
                            m_MessagesIn);

                    if (OnClientConnect(newConn))
                    {
                        m_Connections.push_back(newConn);
                        m_Connections.back()->ConnectToClient(m_IDCounter++);
                        SENNET_CORE_INFO("[Server] Connection {0} approved.",
                            m_Connections.back()->GetID());
                    }
                    else
                    {
                        SENNET_CORE_INFO("[Server] Connection denied.");
                    }
                }
                else
                {
                    SENNET_CORE_ERROR("[Server] New connection error: {0}",
                        ec.message());
                }

                WaitForClientConnection();
            });
    }

    void MessageClient(Ref<Connection<T>> client, const Message<T>& message)
    {
        if (client && client->IsConnected())
        {
            client->Send(message);
        }
        else
        {
            OnClientDisconnect(client);
            client.reset();
            m_Connections.erase(
                std::remove(m_Connections.begin(), m_Connections.end(), client),
                m_Connections.end());
        }
    }

    void MessageAllClients(
        const Message<T>& message, Ref<Connection<T>> ignoreClient = nullptr)
    {
        bool invalidClientExists = false;
        for (auto& client : m_Connections)
        {
            if (client && client->IsConnected())
            {
                if (client != ignoreClient)
                {
                    client->Send(message);
                }
            }
            else
            {
                OnClientDisconnect(client);
                client.reset();
                invalidClientExists = true;
            }
        }

        if (invalidClientExists)
        {
            m_Connections.erase(std::remove(m_Connections.begin(),
                                    m_Connections.end(),
                                    nullptr),
                m_Connections.end());
        }
    }

    void Update(uint64_t maxMessages = -1)
    {
        uint64_t messageCount = 0;
        while (messageCount < maxMessages && !m_MessagesIn.empty())
        {
            auto message = m_MessagesIn.pop_front();
            OnMessage(message.Remote, message.Msg);

            messageCount++;
        }
    }

protected:
    virtual bool OnClientConnect(Ref<Connection<T>> client) { return false; }

    virtual void OnClientDisconnect(Ref<Connection<T>> client) {}

    virtual void OnMessage(Ref<Connection<T>> client, Message<T>& message) {}

protected:
    ThreadSafeQueue<OwnedMessage<T>> m_MessagesIn;

    // Temporary.
    asio::io_context m_Context;
    std::thread m_ContextThread;

    // Temporary.
    asio::ip::tcp::acceptor m_Acceptor;

    uint32_t m_IDCounter = 10000;
    std::deque<Ref<Connection<T>>> m_Connections;
};

} // namespace TCP
} // namespace Sennet
