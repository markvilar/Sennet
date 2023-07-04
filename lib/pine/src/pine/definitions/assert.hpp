#pragma once

#if defined(PINE_DEBUG)
#define PINE_ENABLE_ASSERTS
#elif defined(PINE_RELEASE)
#else
#error "Unknown build type!"
#endif

// TODO: Make macro able to take in no arguments except condition.
#ifdef PINE_ENABLE_ASSERTS
#define PINE_ASSERT(x, ...)                                                    \
    {                                                                          \
        if (!(x))                                                              \
        {                                                                      \
            PINE_ERROR("Assertion failed: {0}", __VA_ARGS__);                  \
            PINE_DEBUGBREAK();                                                 \
        }                                                                      \
    }
#define PINE_CORE_ASSERT(x, ...)                                               \
    {                                                                          \
        if (!(x))                                                              \
        {                                                                      \
            PINE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);             \
            PINE_DEBUGBREAK();                                                 \
        }                                                                      \
    }
#else
#define PINE_ASSERT(x, ...)
#define PINE_CORE_ASSERT(x, ...)
#endif
