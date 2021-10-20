#pragma once

// TODO: Make macro able to take in no arguments except condition.
#ifdef SENNET_ENABLE_ASSERTS
#define SENNET_ASSERT(x, ...)                                                  \
{                                                                              \
    if (!(x))                                                                  \
    {                                                                          \
        SENNET_ERROR("Assertion failed: {0}", __VA_ARGS__);                    \
        SENNET_DEBUGBREAK();                                                   \
    }                                                                          \
}
#define SENNET_CORE_ASSERT(x, ...)                                             \
{                                                                              \
    if (!(x))                                                                  \
    {                                                                          \
        SENNET_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);               \
        SENNET_DEBUGBREAK();                                                   \
    }                                                                          \
}
#else
#define SENNET_ASSERT(x, ...)
#define SENNET_CORE_ASSERT(x, ...)
#endif
