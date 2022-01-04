#pragma once

#include <asio.hpp>

#include "Pine/Network/TCP/Connection.hpp"
#include "Pine/Network/TCP/Message.hpp"
#include "Pine/Network/ThreadSafeQueue.hpp"

namespace Pine::TCP 
{

template <typename T> class Client
{
public:
    Client() {}

    virtual ~Client() { Disconnect(); }

    bool Connect(const std::string& host, const uint16_t& port)
    {
        try
        {
            asio::ip::tcp::resolver resolver(m_Context);
            asio::ip::tcp::resolver::results_type endpoints =
                resolver.resolve(host, std::to_string(port));

            // Create socket. TODO: Add socket options.
            asio::ip::tcp::socket socket(m_Context);

            m_Connection =
                CreateScope<Connection<T>>(Connection<T>::Owner::Client,
                    m_Context,
                    std::move(socket),
                    m_MessagesIn);

            m_Connection->ConnectToServer(endpoints);

            m_ContextThread = std::thread([this]() { m_Context.run(); });
        }
        catch (std::exception& e)
        {
            PINE_CORE_ERROR("[Client] Exception: {0}", e.what());
            return false;
        }
        return true;
    }

    void Disconnect()
    {
        if (IsConnected())
        {
            m_Connection->Disconnect();
        }

        m_Context.stop();
        if (m_ContextThread.joinable())
        {
            m_ContextThread.join();
        }

        m_Connection.release();
    }

    bool IsConnected()
    {
        if (m_Connection)
        {
            return m_Connection->IsConnected();
        }
        else
        {
            return false;
        }
    }

    void Send(const Message<T>& message)
    {
        if (IsConnected())
        {
            m_Connection->Send(message);
        }
    }

    ThreadSafeQueue<OwnedMessage<T>>& Incoming() { return m_MessagesIn; }

protected:
    // TODO: Temporary.
    asio::io_context m_Context;

    std::thread m_ContextThread;

    Scope<Connection<T>> m_Connection;

private:
    ThreadSafeQueue<OwnedMessage<T>> m_MessagesIn;
};

} // namespace Pine::TCP
