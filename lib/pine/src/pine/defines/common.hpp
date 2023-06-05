#pragma once

#if defined(PINE_DEBUG)
#define PINE_ENABLE_ASSERTS
#elif defined(PINE_RELEASE)
#else
#error "Unknown build type!"
#endif

#include "pine/core/log.hpp"
#include "pine/defines/assert.hpp"
#include "pine/defines/platform.hpp"

#define BIT(x) (1 << x)

#define PINE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
