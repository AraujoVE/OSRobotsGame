#include "Log.hpp"

#include "depch.hpp"
namespace DampEngine
{
    Logger::Ref<spdlog::logger> Logger::s_EngineLogger;
    Logger::Ref<spdlog::logger> Logger::s_ClientLogger;

    void Logger::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        auto stdout_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();

        s_EngineLogger = std::make_shared<spdlog::logger>("ENGINE", stdout_sink);
        s_ClientLogger = std::make_shared<spdlog::logger>("APP", stdout_sink);

        s_EngineLogger->set_level(spdlog::level::trace);
        s_ClientLogger->set_level(spdlog::level::trace);

        s_EngineLogger->info("Logger system initialized");
        s_ClientLogger->info("Logger system initialized");

    }
} // namespace DampEngine