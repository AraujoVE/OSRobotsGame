#include "ThreadController.hpp"

#include "mypch.hpp"

#include "Application/Game/GameConsts.hpp"


//TODO: remove
#include "DampEngine/Threads/Mutex.hpp"


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

    void ThreadController::AddIndividual(const Individual &indRun)
    {
        m_QueuedIndividuals.push(indRun);
    }

    std::vector<IndividualRunResult> ThreadController::ExecuteAllIndividuals(ScriptRunner& scriptRunner) {
        std::vector<std::future<IndividualRunResult*>> futureVec;

        std::vector<IndividualRunResult> results;
        results.reserve(m_QueuedIndividuals.size());

        bool forceSync = true;
        
        DampEngine::Mutex gambiarra;
        while (!m_QueuedIndividuals.empty()) {
            Individual &individual = m_QueuedIndividuals.front();

            GameConsts *currentConsts = new GameConsts();
            currentConsts->LoadFromCromossome(individual.Genes);
            currentConsts->SetTickDelay(5e2);
            GameRunner *currentRunner = new GameRunner(currentConsts);


            auto executeAllGameplaysFn = [=, &scriptRunner, &gambiarra, &individual]() {
                //TODO: support more threads per individual (assuming only one for all gameplays)
                // indvRun.scriptRunner->RunGameplay(gamplInd, customGameRunner);

                auto *threadRet = scriptRunner.RunAllGameplays(*currentRunner, individual);

                delete currentRunner;
                delete currentConsts;

                if (forceSync)
                    gambiarra.Unlock();

                return threadRet;
            };

            m_QueuedIndividuals.pop();

            if (forceSync)
                gambiarra.Lock();        

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