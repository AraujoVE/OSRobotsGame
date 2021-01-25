#include "ThreadController.hpp"

#include "mypch.hpp"

namespace EvoAlg
{

    void ThreadController::OnMaxThreadsChanged()
    {
        //TODO: start processing queued stuff
        //TODO?: if maxThreads--, check if it is possible to pause some threads (nonimportant)
    }

    void ThreadController::SetMaxThreads(uint8_t maxThreads)
    {
        if (maxThreads < 2)
            DE_CRITICAL("Currently it's not possible to run this EA with less than 2 threads");

        //A pack is an IndividualThread + m_GameplaysPerIndividual threads
        uint8_t packSize = m_GameplaysPerIndividual + 1;
        if (maxThreads > (m_GameplaysPerIndividual + 1))
        {
            uint8_t threadPacks = maxThreads / packSize;
            m_MaxIndividualThreads = threadPacks;
            m_MaxGameplayThreads = threadPacks * m_GameplaysPerIndividual;
        }
        else
        {
            m_MaxIndividualThreads = 1;
            m_MaxGameplayThreads = maxThreads - 1;
        }
    }

    void ThreadController::AddIndividualRun(const IndividualRun &indRun)
    {
        m_QueuedRuns.push(indRun);
    }

    std::vector<IndividualRunResult> ThreadController::ExecuteRuns() {
        std::vector<std::future<IndividualRunResult*>> futureVec;

        std::vector<IndividualRunResult> results;
        results.reserve(m_QueuedRuns.size());
        
        while (!m_QueuedRuns.empty()) {
            IndividualRun &indvRun = m_QueuedRuns.front();

            GameRunner *gameRunner = indvRun.gameRunner;
            ScriptRunner *scriptRunner = indvRun.scriptRunner;
            Individual *individual = indvRun.individual;

            auto executeAllGameplaysFn = [=]() {
                //TODO: support more threads per individual (assuming only one for all gameplays)
                // indvRun.scriptRunner->RunGameplay(gamplInd, customGameRunner);

                auto *threadRet = scriptRunner->RunAllGameplays();

                delete gameRunner;
                delete scriptRunner;
                delete individual;

                return threadRet;
            };
            
            m_QueuedRuns.pop();

            futureVec.push_back(std::async(executeAllGameplaysFn));
            
        }

        for (auto &future : futureVec) {
            IndividualRunResult *indivResPtr = future.get();
            results.push_back(*indivResPtr);
            delete indivResPtr;
        }

        return results;
    }

} // namespace EvoAlg