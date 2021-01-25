#pragma once

#include <stdint.h>
#include <queue>

namespace EvoAlg
{

    class ThreadController
    {
    public:
        void SetMaxThreads(uint8_t maxThreads) {
            if (maxThreads < 2) DE_FATAL("Currently it's not possible to run this EA with less than 2 threads");

            //A pack is an IndividualThread + m_GameplaysPerIndividual threads
            uint8_t packSize = m_GameplaysPerIndividual + 1;
            if (maxThreads > (m_GameplaysPerIndividual + 1)) {
                uint8_t threadPacks = maxThreads / packSize;
                m_MaxIndividualThreads = threadPacks;
                m_MaxGameplayThreads = threadPacks * m_GameplaysPerIndividual;
            }
            else {
                m_MaxIndividualThreads = 1;
                m_MaxGameplayThreads = maxThreads - 1;
            }
        }

        void StartGameplay();
        void StartIndividualRun();

        void WaitForAllRuns();

    private:

        void onMaxThreadsChanged() {
            //TODO: start processing queued stuff
            //TODO?: if maxThreads--, check if it is possible to pause some threads (nonimportant)
        }

        std::queue<IndividualPackOrSomethingLikeThat> queuedOperations;
        uint8_t m_CurrentRunningThreads = 0;

        //TODO: get this dinamically
        uint16_t m_GameplaysPerIndividual = 4;

        uint8_t m_MaxIndividualThreads = 1;
        uint8_t m_MaxGameplayThreads = 1;
    };

} // namespace EvoAlg