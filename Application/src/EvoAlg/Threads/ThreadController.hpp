#pragma once

#include "EvoAlg/Types.hpp"
#include "EvoAlg/ScriptRunner.hpp"
#include "Application/Game/GameRunner.hpp"

#include <stdint.h>
#include <queue>

namespace EvoAlg
{
    struct IndividualRun
    {
        Individual *individual;
        Application::GameRunner *gameRunner;
        ScriptRunner *scriptRunner;
    };

    class ThreadController
    {
    public:
        void SetMaxThreads(uint8_t maxThreads);
        inline uint8_t GetMaxThreads() { return m_MaxIndividualThreads + m_MaxGameplayThreads; }
        void AddIndividualRun(const IndividualRun& indRun);

        std::vector<IndividualRunResult> ExecuteRuns();

    private:

        void OnMaxThreadsChanged();

        
        std::queue<IndividualRun> m_QueuedRuns;
        uint8_t m_CurrentRunningThreads = 0;

        //TODO: get this dinamically
        uint16_t m_GameplaysPerIndividual = 4;

        uint8_t m_MaxIndividualThreads = 1;
        uint8_t m_MaxGameplayThreads = 1;
    };

} // namespace EvoAlg