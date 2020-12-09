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
