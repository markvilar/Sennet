#pragma once

namespace Sennet
{

// TODO: Windows: WSA initialization

enum class NetworkProtocol
{
    IPv4,
    IPv6
};

class Socket
{
public:
    enum class Type
    {
        Raw,
        Stream,
        Datagram
    };

    struct Specification
    {
        NetworkProtocol Protocol = NetworkProtocol::IPv4;
        Type SocketType = Type::Stream;

        bool Blocking = false;
        bool KeepAlive = false;
        bool ReuseAddress = false;
        bool ReusePort = false;
        float Timeout = 1000;
    };

    virtual ~Socket() = default;

    // Generic methods.
    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual size_t Send(const void* data, size_t size) = 0;
    virtual size_t Receive(void* data, size_t size) = 0;

    // Server methods.
    virtual void Bind(const std::string_view host, const uint16_t port) = 0;
    virtual void Listen() = 0;
    virtual void Accept(const std::string_view host, const uint16_t port) = 0;

    // Client methods.
    virtual void Connect(const std::string_view host, const uint16_t port) = 0;
    virtual void Disconnect() = 0;

    // Status methods.
    virtual bool IsBound() = 0;
    virtual bool IsConnected() = 0;
    virtual bool IsOpen() = 0;

    static Ref<Socket> Create(const Specification& specs);

private:
    Specification m_Specifications;
};

} // namespace Sennet
