#pragma once

#if defined(PINE_DEBUG)
#define PINE_ENABLE_ASSERTS
#elif defined(PINE_RELEASE)
#else
#error "Unknown build type!"
#endif

#include "Pine/Core/Assert.hpp"
#include "Pine/Core/Log.hpp"
#include "Pine/Core/Platform.hpp"

#define BIT(x) (1 << x)

#define PINE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Pine
{

using byte = uint8_t;

} // namespace Pine
