#pragma once

#if defined(PINE_DEBUG)
#define PINE_ENABLE_ASSERTS
#elif defined(PINE_RELEASE)
#else
#error "Unknown build type!"
#endif

#include "pine/core/assert.hpp"
#include "pine/core/log.hpp"
#include "pine/core/platform.hpp"
