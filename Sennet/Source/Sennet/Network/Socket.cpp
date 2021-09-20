#include "Sennet/Pch.hpp"
#include "Sennet/Network/Socket.hpp"

#if defined(SN_PLATFORM_LINUX)
#include "Platform/Linux/LinuxSocket.hpp"
#endif

namespace Sennet
{

Ref<Socket> Socket::Create(const Socket::Specification& specs)
{
#if defined(SN_PLATFORM_LINUX)
    return CreateRef<LinuxSocket>(specs);
#else
	SN_CORE_ASSERT(false, "Unsupported platform!");
	return nullptr;
#endif
}

}
