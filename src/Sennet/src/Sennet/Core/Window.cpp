#include "Sennet/Core/Window.hpp"
#include "Sennet/Pch.hpp"

#if defined(SENNET_PLATFORM_WINDOWS)
#include "Sennet/Platform/Windows/WindowsWindow.hpp"
#elif defined(SENNET_PLATFORM_LINUX)
#include "Sennet/Platform/Linux/LinuxWindow.hpp"
#endif

namespace Sennet
{

Scope<Window> Window::Create(const WindowProperties& props)
{
#if defined(SENNET_PLATFORM_WINDOWS)
    return CreateScope<WindowsWindow>(props);
#elif defined(SENNET_PLATFORM_LINUX)
    return CreateScope<LinuxWindow>(props);
#else
    SENNET_CORE_ASSERT(false, "Unsupported platform!");
    return nullptr;
#endif
}

} // namespace Sennet
