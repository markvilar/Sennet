#pragma once

#if defined(SENNET_DEBUG)
#define SENNET_ENABLE_ASSERTS
#elif defined(SENNET_RELEASE)
#else
#error "Unknown build type!"
#endif

#include "Sennet/Core/Assert.hpp"
#include "Sennet/Core/Platform.hpp"

#include <memory>

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
