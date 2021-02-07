#pragma once

#include "EvoAlg/Types.hpp"
#include "EvoAlg/ScriptRunner.hpp"
#include "Application/Game/GameRunner.hpp"

#include <stdint.h>
#include <queue>

namespace EvoAlg
{
    class ThreadController
    {
    public:
        void SetMaxThreads(uint8_t maxThreads);
        inline uint8_t GetMaxThreads() { return m_MaxIndividualThreads + m_MaxGameplayThreads; }
        void AddIndividual(const Individual& indRun);

        std::vector<IndividualRunResult> ExecuteAllIndividuals(ScriptRunner& scriptRunner);

    private:

        void OnMaxThreadsChanged();

        
        std::queue<Individual> m_QueuedIndividuals;
        uint8_t m_CurrentRunningThreads = 0;

        //TODO: get this dinamically
        uint16_t m_GameplaysPerIndividual = 4;

        uint8_t m_MaxIndividualThreads = 1;
        uint8_t m_MaxGameplayThreads = 1;
    };

} // namespace EvoAlg