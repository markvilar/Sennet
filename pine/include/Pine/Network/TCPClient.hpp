#pragma once

#include <memory>

#include <asio.hpp>

#include "Pine/Network/Connection.hpp"
#include "Pine/Network/Message.hpp"
#include "Pine/Network/ThreadSafeQueue.hpp"

namespace Pine
{

template <typename T>
class TCPClient
{
    using tcp = asio::ip::tcp;

public:
    TCPClient() {}

    virtual ~TCPClient() { Disconnect(); }

    bool Connect(const std::string& host, const uint16_t port)
    {
        try
        {
            tcp::resolver resolver(m_Context);
            tcp::socket socket(m_Context);
            auto endpoints = resolver.resolve(host, std::to_string(port));

            m_Connection =
                std::make_unique<Connection<T>>(Connection<T>::Owner::Client,
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
        return m_Connection ? m_Connection->IsConnected() : false;
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
    asio::io_context m_Context;
    std::thread m_ContextThread;
    std::unique_ptr<Connection<T>> m_Connection;

private:
    ThreadSafeQueue<OwnedMessage<T>> m_MessagesIn;
};

} // namespace Pine
