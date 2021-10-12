#pragma once

#include <memory>

#if defined(SENNET_PLATFORM_LINUX)
#elif defined(SENNET_PLATFORM_WINDOWS)
#define _WIN32_WINNT 0x0601
#include <asio.hpp>
#include <windows.h>
#elif defined(SENNET_PLATFORM_MACOS)
#error "MACOS is not supported yet!"
#elif defined(SENNET_PLATFORM_IOS)
#error "IOS is not supported yet!"
#else
#error "Unknown build platform!"
#endif

#if defined(SENNET_DEBUG)
#elif defined(SENNET_RELEASE)
#else
#error "Unknown build type!"
#endif

#if defined(SENNET_DEBUG)
#if defined(SENNET_PLATFORM_WINDOWS)
#define SENNET_DEBUGBREAK() __debugbreak()
#elif defined(SENNET_PLATFORM_LINUX)
#include <signal.h>
#define SENNET_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define SENNET_ENABLE_ASSERTS
#elif defined(SENNET_RELEASE)
#define SENNET_DEBUGBREAK()
#endif

// TODO: Make macro able to take in no arguments except condition.
#ifdef SENNET_ENABLE_ASSERTS
#define SENNET_ASSERT(x, ...)                                                  \
    {                                                                          \
        if (!(x))                                                              \
        {                                                                      \
            SENNET_ERROR("Assertion failed: {0}", __VA_ARGS__);                \
            SENNET_DEBUGBREAK();                                               \
        }                                                                      \
    }
#define SENNET_CORE_ASSERT(x, ...)                                             \
    {                                                                          \
        if (!(x))                                                              \
        {                                                                      \
            SENNET_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);           \
            SENNET_DEBUGBREAK();                                               \
        }                                                                      \
    }
#else
#define SENNET_ASSERT(x, ...)
#define SENNET_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SENNET_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Sennet
{

template <typename T> using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T> using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

using byte = uint8_t;

} // namespace Sennet