#pragma once

#include "Pine/Network/Socket.hpp"

namespace Pine
{

class LinuxSocket : public Socket
{
public:
    LinuxSocket(const Socket::Specification& specs);
    virtual ~LinuxSocket() = default;

    // Generic methods.
    virtual void Open() override;
    virtual void Close() override;
    virtual size_t Send(const void* data, size_t size) override;
    virtual size_t Receive(void* data, size_t size) override;

    // Server methods.
    virtual void Bind(
        const std::string_view host, const uint16_t port) override;
    virtual void Listen() override;
    virtual void Accept(
        const std::string_view host, const uint16_t port) override;

    // Client methods.
    virtual void Connect(
        const std::string_view host, const uint16_t port) override;
    virtual void Disconnect() override;

    // Status methods.
    virtual bool IsBound() override;
    virtual bool IsConnected() override;
    virtual bool IsOpen() override;

private:
    int m_ID = -1;
};

} // namespace Pine
