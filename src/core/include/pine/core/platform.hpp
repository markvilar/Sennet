#pragma once

#if defined(PINE_PLATFORM_LINUX)
#elif defined(PINE_PLATFORM_WINDOWS)
	#if not defined(_WIN32_WINNT) 
		#define _WIN32_WINNT 0x601
	#endif
	#include <WinSock2.h>
	#include <Windows.h>
	#include <asio.hpp>
#elif defined(PINE_PLATFORM_MACOS)
#error "MACOS is not supported yet!"
#elif defined(PINE_PLATFORM_IOS)
#error "IOS is not supported yet!"
#else
#error "Unknown build platform!"
#endif

#if defined(PINE_DEBUG)
#if defined(PINE_PLATFORM_WINDOWS)
#define PINE_DEBUGBREAK() __debugbreak()
#elif defined(PINE_PLATFORM_LINUX)
#include <signal.h>
#define PINE_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define PINE_ENABLE_ASSERTS
#elif defined(PINE_RELEASE)
#define PINE_DEBUGBREAK()
#endif
