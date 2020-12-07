#include "Log.h"

namespace DampEngine
{
    Logger::Ref<spdlog::logger> Logger::s_EngineLogger;
    Logger::Ref<spdlog::logger> Logger::s_ClientLogger;

    void Logger::Init()
    {

        s_EngineLogger = std::make_shared<spdlog::logger>("ENGINE");
        spdlog::register_logger(s_EngineLogger);
        s_EngineLogger->set_level(spdlog::level::trace);
        s_EngineLogger->flush_on(spdlog::level::trace);
        
        s_ClientLogger = std::make_shared<spdlog::logger>("APP");
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);
    }
} // namespace DampEngine