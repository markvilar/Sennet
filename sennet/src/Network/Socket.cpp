#include "Sennet/Network/Socket.hpp"
#include "Sennet/Pch.hpp"

#if defined(SENNET_PLATFORM_LINUX)
#include "Sennet/Platform/Linux/LinuxSocket.hpp"
#endif

namespace Sennet
{

Ref<Socket> Socket::Create(const Socket::Specification& specs)
{
#if defined(SENNET_PLATFORM_LINUX)
    return CreateRef<LinuxSocket>(specs);
#else
    SENNET_CORE_ASSERT(false, "Unsupported platform!");
    return nullptr;
#endif
}

} // namespace Sennet
