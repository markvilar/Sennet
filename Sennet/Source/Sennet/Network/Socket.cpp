#include "Sennet/Pch.hpp"
#include "Sennet/Network/Socket.hpp"

#if defined(SENNET_PLATFORM_LINUX)
#include "Platform/Linux/LinuxSocket.hpp"
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

}
