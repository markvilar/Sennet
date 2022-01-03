#include "Sennet/Core/Log.hpp"
#include "Sennet/Pch.hpp"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Sennet
{

Ref<spdlog::logger> Log::s_CoreLogger;
Ref<spdlog::logger> Log::s_ClientLogger;

void Log::Init()
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.emplace_back(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>("Sennet.log",
            true));

    sinks[0]->set_pattern("%^[%T] %n: %v%$");
    sinks[1]->set_pattern("[%T] [%l] %n: %v");

    // Core logger.
    s_CoreLogger =
        std::make_shared<spdlog::logger>("SENNET", begin(sinks), end(sinks));
    spdlog::register_logger(s_CoreLogger);
    s_CoreLogger->set_level(spdlog::level::trace);
    s_CoreLogger->flush_on(spdlog::level::trace);

    // Client logger.
    s_ClientLogger =
        std::make_shared<spdlog::logger>("APP", begin(sinks), end(sinks));
    spdlog::register_logger(s_ClientLogger);
    s_ClientLogger->set_level(spdlog::level::trace);
    s_ClientLogger->flush_on(spdlog::level::trace);
}

} // namespace Sennet
