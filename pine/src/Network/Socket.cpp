#include "Pine/Network/Socket.hpp"
#include "Pine/Pch.hpp"

#if defined(PINE_PLATFORM_LINUX)
#include "Pine/Platform/Linux/LinuxSocket.hpp"
#endif

namespace Pine
{

Ref<Socket> Socket::Create(const Socket::Specification& specs)
{
#if defined(PINE_PLATFORM_LINUX)
    return CreateRef<LinuxSocket>(specs);
#else
    PINE_CORE_ASSERT(false, "Unsupported platform!");
    return nullptr;
#endif
}

} // namespace Pine
