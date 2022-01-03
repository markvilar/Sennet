#pragma once

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
