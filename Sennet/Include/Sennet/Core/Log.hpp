#pragma once

#include "Sennet/Core/Base.hpp"

#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace Sennet
{

class Log
{
public:
    static void Init();

    static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
    static Ref<spdlog::logger> s_CoreLogger;
    static Ref<spdlog::logger> s_ClientLogger;
};

} // namespace Sennet

// Core log macros.
#define SN_CORE_TRACE(...) ::Sennet::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SN_CORE_INFO(...) ::Sennet::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SN_CORE_WARN(...) ::Sennet::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SN_CORE_ERROR(...) ::Sennet::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SN_CORE_CRITICAL(...)                                                  \
    ::Sennet::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros.
#define SN_TRACE(...) ::Sennet::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SN_INFO(...) ::Sennet::Log::GetClientLogger()->info(__VA_ARGS__)
#define SN_WARN(...) ::Sennet::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SN_ERROR(...) ::Sennet::Log::GetClientLogger()->error(__VA_ARGS__)
#define SN_CRITICAL(...) ::Sennet::Log::GetClientLogger()->critical(__VA_ARGS__)
