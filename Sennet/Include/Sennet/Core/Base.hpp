#pragma once

#include <memory>

#if defined(SN_PLATFORM_LINUX)
#elif defined(SN_PLATFORM_WINDOWS)
#elif defined(SN_PLATFORM_MACOS)
    #error "MACOS is not supported yet!"
#elif defined(SN_PLATFORM_IOS)
    #error "IOS is not supported yet!"
#else
    #error "Unknown build platform!"
#endif

#if defined(SN_DEBUG)
#elif defined(SN_RELEASE)
#else
    #error "Unknown build type!"
#endif

#if defined(SN_DEBUG)
    #if defined(SN_PLATFORM_WINDOWS)
        #define SN_DEBUGBREAK() __debugbreak()
    #elif defined(SN_PLATFORM_LINUX)
        #include <signal.h>
        #define SN_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define SN_ENABLE_ASSERTS
#elif defined(SN_RELEASE)
    #define SN_DEBUGBREAK()
#endif

// TODO: Make macro able to take in no arguments except condition.
#ifdef SN_ENABLE_ASSERTS
#define SN_ASSERT(x, ...)                                                      \
    {                                                                          \
        if (!(x))                                                              \
        {                                                                      \
            SN_ERROR("Assertion failed: {0}", __VA_ARGS__);                    \
            SN_DEBUGBREAK();                                                   \
        }                                                                      \
    }
#define SN_CORE_ASSERT(x, ...)                                                 \
    {                                                                          \
        if (!(x))                                                              \
        {                                                                      \
            SN_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);               \
            SN_DEBUGBREAK();                                                   \
        }                                                                      \
    }
#else
#define SN_ASSERT(x, ...)
#define SN_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SN_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

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
