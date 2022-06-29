#pragma once

#include <memory>

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace pine
{

class Log
{
public:
    static void Init();

    static const std::shared_ptr<spdlog::logger>& GetCoreLogger()
    {
        return s_CoreLogger;
    }

    static const std::shared_ptr<spdlog::logger>& GetClientLogger()
    {
        return s_ClientLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

} // namespace pine

// Core log macros.
#define PINE_CORE_TRACE(...) ::pine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PINE_CORE_INFO(...) ::pine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PINE_CORE_WARN(...) ::pine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PINE_CORE_ERROR(...) ::pine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PINE_CORE_CRITICAL(...)                                                \
    ::pine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros.
#define PINE_TRACE(...) ::pine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PINE_INFO(...) ::pine::Log::GetClientLogger()->info(__VA_ARGS__)
#define PINE_WARN(...) ::pine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PINE_ERROR(...) ::pine::Log::GetClientLogger()->error(__VA_ARGS__)
#define PINE_CRITICAL(...) ::pine::Log::GetClientLogger()->critical(__VA_ARGS__)
