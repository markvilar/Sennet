#include "Pine/Network/TCP.hpp"

namespace Pine::TCP
{

Connection::Connection(Owner owner, asio::io_context& context,
    tcp::socket socket,
    ThreadSafeQueue<std::pair<ConnectionPtr, Message>>& messagesIn)
    : m_Owner(owner), m_Context(context), m_Socket(std::move(socket)),
      m_InBuffers(messagesIn)
{
}

void Connection::ConnectToClient(const uint32_t id)
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

void Connection::ConnectToServer(const tcp::resolver::results_type& endpoints)
{
    if (m_Owner == Owner::Server)
    {
        return;
    }
        
    // TODO: Set up timeout.
    asio::async_connect(m_Socket,
        endpoints,
        [this](const std::error_code error, const tcp::endpoint endpoint) {
            if (error)
            {
                PINE_CORE_ERROR("[{0}] Connect to server failed: {1}",
                    m_ID,
                    error.message());
                return;
            }

            ReadHeader();
        });
}

void Connection::Disconnect()
{
    if (IsConnected())
    {
        asio::post(m_Context, [this]() { m_Socket.close(); });
    }
}

void Connection::Send(const Message& message)
{
    asio::post(m_Context, [this, message]() {
        const auto writingMessages = !m_OutBuffers.empty();
        m_OutBuffers.push_back(message);
        if (!writingMessages)
        {
            WriteHeader();
        }
    });
}

void Connection::ReadHeader()
{
    auto message = std::make_shared<Message>();

    asio::async_read(m_Socket,
        asio::buffer(&message->Header, sizeof(MessageHeader)),
        [this, message](const std::error_code error, const uint64_t length) {
            if (error)
            {
                PINE_CORE_ERROR("[{0}] Reading message header failed: {1}",
                    m_ID,
                    error.message());
                m_Socket.close();
                return;
            }

            if (message->Header.Size > 0)
            {
                message->Body.resize(message->Header.Size);
                ReadBody(message);
            }
        });
}

void Connection::ReadBody(const std::shared_ptr<Message>& message)
{
    asio::async_read(m_Socket,
        asio::buffer(message->Body.data(), message->Body.size()),
        [this, message](const std::error_code error, const uint64_t length) {
            if (error)
            {
                PINE_CORE_ERROR("[{0}] Reading message body failed: {1}",
                    m_ID,
                    error.message());
                m_Socket.close();
                return;
            }

            AddToInQueue(*message.get());
        });
}

void Connection::WriteHeader()
{
    asio::async_write(m_Socket,
        asio::buffer(&m_OutBuffers.front().Header, sizeof(MessageHeader)),
        [this](const std::error_code error, const uint64_t length) {
            if (error)
            {
                PINE_CORE_ERROR("[{0}] Writing message header failed: {1}",
                    m_ID,
                    error.message());
                m_Socket.close();
                return;
            }

            if (m_OutBuffers.front().Body.size() > 0)
            {
                WriteBody();
            }
            else
            {
                m_OutBuffers.pop_front();
                if (!m_OutBuffers.empty())
                {
                    WriteHeader();
                }
            }
        });
}

void Connection::WriteBody()
{
    asio::async_write(m_Socket,
        asio::buffer(m_OutBuffers.front().Body.data(),
            m_OutBuffers.front().Body.size()),
        [this](const std::error_code error, const uint64_t length) {
            if (error)
            {
                PINE_CORE_ERROR("[{0}] Writing message body failed: {1}",
                    m_ID,
                    error.message());
                m_Socket.close();
                return;
            }

            m_OutBuffers.pop_front();
            if (!m_OutBuffers.empty())
            {
                WriteHeader();
            }
        });
}

void Connection::AddToInQueue(const Message& message)
{
    if (m_Owner == Owner::Server)
    {
        m_InBuffers.push_back({this->shared_from_this(), std::move(message)});
    }
    else
    {
        m_InBuffers.push_back({nullptr, std::move(message)});
    }

    ReadHeader();
}

bool Client::Connect(const std::string& host, const uint16_t port)
{
    if (m_ContextThread.joinable())
    {
        return false;
    }

    tcp::resolver resolver(m_Context);
    tcp::socket socket(m_Context);
    const auto endpoints = resolver.resolve(host, std::to_string(port));

    m_Connection = std::make_unique<Connection>(Connection::Owner::Client,
        m_Context,
        std::move(socket),
        m_MessagesIn);
    m_Connection->ConnectToServer(endpoints);
    m_ContextThread = std::thread([this]() { m_Context.run(); });
    return true;
}

void Client::Disconnect()
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

bool Client::IsConnected()
{
    return m_Connection ? m_Connection->IsConnected() : false;
}

void Client::Send(const Message& message)
{
    if (IsConnected())
    {
        m_Connection->Send(message);
    }
}

const ThreadSafeQueue<std::pair<std::shared_ptr<Connection>, Message>>&
Client::Incoming()
{
    return m_MessagesIn;
}

Server::Server(const uint16_t port)
    : m_Acceptor(m_Context, tcp::endpoint(tcp::v4(), port))
{
}

bool Server::Start()
{
    try
    {
        Listen();
        m_ContextThread = std::thread([this]() { m_Context.run(); });
    }
    catch (const std::exception& e)
    {
        PINE_CORE_ERROR("Server exception: {0}", e.what());
        return false;
    }
    return true;
}

void Server::Stop()
{
    m_Context.stop();

    if (m_ContextThread.joinable())
    {
        m_ContextThread.join();
    }
}

void Server::Listen()
{
    m_Acceptor.async_accept(
        [this](const std::error_code ec, tcp::socket socket) {
            if (!ec)
            {
                PINE_CORE_INFO("New connection: {0}", socket.remote_endpoint());

                auto newConnection =
                    std::make_shared<Connection>(Connection::Owner::Server,
                        m_Context,
                        std::move(socket),
                        m_MessagesIn);

                if (OnClientConnect(newConnection))
                {
                    m_Connections.push_back(newConnection);
                    m_Connections.back()->ConnectToClient(m_IDCounter++);
                }
                else
                {
                    PINE_CORE_INFO("Connection denied: {0}",
                        socket.remote_endpoint());
                }
            }
            else
            {
                PINE_CORE_ERROR("Server error: {0}", ec.message());
            }

            Listen();
        });
}

void Server::MessageClient(const ConnectionPtr& client, const Message& message)
{
    if (client && client->IsConnected())
    {
        client->Send(message);
    }
    else
    {
        OnClientDisconnect(client);
        //client.reset();
        m_Connections.erase(
            std::remove(m_Connections.begin(), m_Connections.end(), client),
            m_Connections.end());
    }
}

void Server::Update(const uint64_t maxMessages)
{
    uint64_t messageCount = 0;
    while (messageCount < maxMessages && !m_MessagesIn.empty())
    {
        auto [client, message] = m_MessagesIn.pop_front();
        OnMessage(client, message);
        messageCount++;
    }
}

}; // namespace Pine::TCP
