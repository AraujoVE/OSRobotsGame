#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace DampEngine
{
    class Logger
    {
    private:
    public:
        template <typename T>
        using Ref = std::shared_ptr<T>;
        static void Init();
        
        inline static Ref<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
        inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static Ref<spdlog::logger> s_EngineLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };

} // namespace DampEngine

#define DE_ENGINE_DEBUG(...) DampEngine::Logger::GetEngineLogger()->debug(__VA_ARGS__) 
#define DE_ENGINE_TRACE(...) DampEngine::Logger::GetEngineLogger()->trace(__VA_ARGS__) 
#define DE_ENGINE_INFO(...) DampEngine::Logger::GetEngineLogger()->info(__VA_ARGS__) 
#define DE_ENGINE_WARN(...) DampEngine::Logger::GetEngineLogger()->warn(__VA_ARGS__) 
#define DE_ENGINE_ERROR(...) DampEngine::Logger::GetEngineLogger()->error(__VA_ARGS__) 
#define DE_ENGINE_CRITICAL(...) DampEngine::Logger::GetEngineLogger()->critical(__VA_ARGS__) 

#define DE_DEBUG(...) DampEngine::Logger::GetClientLogger()->debug(__VA_ARGS__) 
#define DE_TRACE(...) DampEngine::Logger::GetClientLogger()->trace(__VA_ARGS__) 
#define DE_INFO(...) DampEngine::Logger::GetClientLogger()->info(__VA_ARGS__) 
#define DE_WARN(...) DampEngine::Logger::GetClientLogger()->warn(__VA_ARGS__) 
#define DE_ERROR(...) DampEngine::Logger::GetClientLogger()->error(__VA_ARGS__) 
#define DE_CRITICAL(...) DampEngine::Logger::GetClientLogger()->critical(__VA_ARGS__) 