#pragma once

#include <memory>
#include <utility>

#include <asio.hpp>

#include "Pine/Core/Base.hpp"
#include "Pine/Network/Message.hpp"
#include "Pine/Utils/ThreadSafeQueue.hpp"

namespace Pine::TCP
{

class Connection : public std::enable_shared_from_this<Connection>
{
    using tcp = asio::ip::tcp;
    using ConnectionPtr = std::shared_ptr<Connection>;

public:
    enum class Owner
    {
        Server,
        Client
    };

    Connection(Owner owner, asio::io_context& context, tcp::socket socket,
        ThreadSafeQueue<std::pair<ConnectionPtr, Message>>& messagesIn);

    virtual ~Connection() = default;

    uint32_t GetID() const { return m_ID; }

    void ConnectToClient(const uint32_t id = 0);
    void ConnectToServer(const tcp::resolver::results_type& endpoints);
    void Disconnect();
    bool IsConnected() const { return m_Socket.is_open(); }

    void Send(const Message& message);

private:
    void ReadHeader();
    void ReadBody(const std::shared_ptr<Message>& message);
    void AddToInQueue(const Message& message);

    void WriteHeader();
    void WriteBody();

protected:
    tcp::socket m_Socket;
    asio::io_context& m_Context;

    Owner m_Owner = Owner::Server;
    uint32_t m_ID = 0;

    ThreadSafeQueue<Message> m_OutBuffers;
    ThreadSafeQueue<std::pair<ConnectionPtr, Message>>& m_InBuffers;
};

class Client
{
    using tcp = asio::ip::tcp;
    using ConnectionPtr = std::shared_ptr<Connection>;

public:
    Client() = default;

    virtual ~Client() { Disconnect(); }

    bool Connect(const std::string& host, const uint16_t port);
    void Disconnect();
    bool IsConnected();

    void Send(const Message& message);

    const ThreadSafeQueue<std::pair<ConnectionPtr, Message>>& Incoming();

protected:
    asio::io_context m_Context;
    std::thread m_ContextThread;
    std::unique_ptr<Connection> m_Connection;

private:
    ThreadSafeQueue<std::pair<ConnectionPtr, Message>> m_MessagesIn;
};

class Server
{
    using tcp = asio::ip::tcp;
    using ConnectionPtr = std::shared_ptr<Connection>;

public:
    Server(const uint16_t port);

    virtual ~Server() { Stop(); }

    bool Start();
    void Stop();

    void Listen();

    void MessageClient(const ConnectionPtr& client, const Message& message);

    void Update(const uint64_t maxMessages = -1);

protected:
    virtual bool OnClientConnect(const ConnectionPtr& client) { return false; }

    virtual void OnClientDisconnect(const ConnectionPtr& client) {}

    virtual void OnMessage(const ConnectionPtr& client, const Message& message)
    {
    }

protected:
    asio::io_context m_Context;
    std::thread m_ContextThread;

    tcp::acceptor m_Acceptor;

    uint32_t m_IDCounter = 10000;
    std::deque<ConnectionPtr> m_Connections;
    ThreadSafeQueue<std::pair<ConnectionPtr, Message>> m_MessagesIn;
};

}; // namespace Pine::TCP
