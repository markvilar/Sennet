#pragma once

#include <memory>

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace pine
{
class Log
{
public:
    static void init();

    static const std::shared_ptr<spdlog::logger>& get_core_logger()
    {
        return s_core_logger;
    }

    static const std::shared_ptr<spdlog::logger>& get_client_logger()
    {
        return s_client_logger;
    }

private:
    static std::shared_ptr<spdlog::logger> s_core_logger;
    static std::shared_ptr<spdlog::logger> s_client_logger;
};

} // namespace pine

// Core log macros.
#define PINE_CORE_TRACE(...) ::pine::Log::get_core_logger()->trace(__VA_ARGS__)
#define PINE_CORE_INFO(...) ::pine::Log::get_core_logger()->info(__VA_ARGS__)
#define PINE_CORE_WARN(...) ::pine::Log::get_core_logger()->warn(__VA_ARGS__)
#define PINE_CORE_ERROR(...) ::pine::Log::get_core_logger()->error(__VA_ARGS__)
#define PINE_CORE_CRITICAL(...)                                                \
    ::pine::Log::get_core_logger()->critical(__VA_ARGS__)

// Client log macros.
#define PINE_TRACE(...) ::pine::Log::get_client_logger()->trace(__VA_ARGS__)
#define PINE_INFO(...) ::pine::Log::get_client_logger()->info(__VA_ARGS__)
#define PINE_WARN(...) ::pine::Log::get_client_logger()->warn(__VA_ARGS__)
#define PINE_ERROR(...) ::pine::Log::get_client_logger()->error(__VA_ARGS__)
#define PINE_CRITICAL(...)                                                     \
    ::pine::Log::get_client_logger()->critical(__VA_ARGS__)
