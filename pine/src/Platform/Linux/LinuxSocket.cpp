#if defined(PINE_PLATFORM_LINUX)
#include "Pine/Platform/Linux/LinuxSocket.hpp"

#include <netinet/in.h> // Data structures for internet domain addresses.
#include <sys/socket.h> // Data structures needed for sockets.
#include <sys/types.h>  // Data types for system calls.
#include <unistd.h>     // close

namespace Pine
{

LinuxSocket::LinuxSocket(const Socket::Specification& specs)
{
    int domain = AF_INET;
    int socketType = SOCK_STREAM | SOCK_NONBLOCK;
    int opt = 1;

    // Create file descriptor.
    m_ID = socket(domain, socketType, 0);
    PINE_CORE_ASSERT(m_ID >= 0, "Error occured while creating socket.");

    setsockopt(m_ID,
        SOL_SOCKET,
        SO_REUSEADDR | SO_REUSEPORT,
        &opt,
        sizeof(opt));
}

void LinuxSocket::Open()
{
    // TODO: int open(const char *pathname, int flags, mode_t mode);
}

void LinuxSocket::Close() { close(m_ID); }

size_t LinuxSocket::Send(const void* data, size_t size)
{
    return send(m_ID, data, size, 0); // TODO: Flags.
}

size_t LinuxSocket::Receive(void* data, size_t size)
{
    return recv(m_ID, data, size, 0); // TODO: Flags.
}

// ----------------------------------------------------------------------------
// ---- Server methods --------------------------------------------------------
// ----------------------------------------------------------------------------

void LinuxSocket::Bind(const std::string_view host, const uint16_t port)
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // TODO: Parse host.
    address.sin_port = htons(port);

    auto bindStatus = bind(m_ID, (struct sockaddr*)&address, sizeof(address));
    PINE_CORE_ASSERT(bindStatus == 0, "Error occured while binding socket.");
}

void LinuxSocket::Listen()
{
    int backlog = 3;
    auto listenStatus = listen(m_ID, backlog);
    PINE_CORE_ASSERT(listenStatus == 0,
        "Error occured while listening on socket.");
}

void LinuxSocket::Accept(const std::string_view host, const uint16_t port)
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // TODO: Parse host.
    address.sin_port = htons(port);
    auto addressLength = sizeof(address);

    auto remote =
        accept(m_ID, (struct sockaddr*)&address, (socklen_t*)&addressLength);
    PINE_CORE_ASSERT(remote >= 0, "Error occured while accepting connections.");
}

// ----------------------------------------------------------------------------
// ---- Client methods --------------------------------------------------------
// ----------------------------------------------------------------------------

void LinuxSocket::Connect(const std::string_view host, const uint16_t port)
{
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // TODO: Parse host.
    serverAddress.sin_port = htons(port);

    auto remote =
        connect(m_ID, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    PINE_CORE_ASSERT(remote >= 0, "Error occured while connecting to server.");
}

void LinuxSocket::Disconnect()
{
    // TODO: Shutdown flag?
    auto status = shutdown(m_ID, SHUT_RDWR);
}

bool LinuxSocket::IsBound() { return false; }

bool LinuxSocket::IsConnected() { return false; }

bool LinuxSocket::IsOpen() { return false; }

} // namespace Pine

#endif
