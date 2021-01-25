#include "Application/Game/GameConsts.hpp"

#include "EvoAlg/EAController.hpp"

#include "EvoAlg/ScriptConverter.hpp"
#include "EvoAlg/ScriptRunner.hpp"

#include "Application/Util/path.hpp"

#include "Application/Threads/Action.hpp"

#include "mypch.hpp"

#include "EvoAlg/Types.hpp"
#include "EvoAlg/Threads/ThreadController.hpp"

namespace EvoAlg
{
    EAController::EAController(EAGuiProps &guiProps) : m_Algorithm(*this), m_Script(nullptr), m_GuiProps(guiProps)
    {
    }

    void EAController::Start()
    {
        DE_ASSERT(m_Status.m_ExecutionInfo.Stage == EAStage::INACTIVE, "(EAController) Trying to start twice");
        ScriptConverter converter(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_SCRIPT_HUMAN_FOLDER));
        m_Script = converter.Convert();

        //m_Script.Save()

        Application::Action<> rpig([this] {
            m_Algorithm.startAlgorithm();
        });
        rpig.Invoke();
    }

    void EAController::Cancel()
    {
        DE_ASSERT(m_Status.m_ExecutionInfo.Stage != EAStage::INACTIVE, "(EAController) Trying to cancel a inactive controller");
    }

    //TODO: Sync or Async UI
    //TODO: assert is being called from only one thread (maybe mutex)
    std::vector<IndividualRunResult> EAController::RunPopulationInGame(const std::vector<GeneVec> &populationGenes)
    {
        DE_ASSERT(m_Script != nullptr);

        DE_DEBUG("EAController::RunPopulationInGame()");
        //Index of array corresponds to IndividualID
        

        ThreadController threadController;

        //TODO: change
        m_GuiProps.MainGameRunner = nullptr;

        //TODO: event EH_GGuiGameAttached (wait for UI to be ready (only if ShowGame is true))
        // usleep(1);

        std::vector<void*> pointersPendingDeletion;
        
        for (unsigned int i = 0; i < populationGenes.size(); i++)
        {   
            GameConsts *currentGameConsts = new GameConsts();
            currentGameConsts->LoadFromCromossome(populationGenes[i]);
            currentGameConsts->SetTickDelay(5e2);


            Individual *currentIndividual = new Individual{i, populationGenes[i]};
            GameRunner *currentGameRunner = new GameRunner(currentGameConsts);
            ScriptRunner *currentScriptRunner = new ScriptRunner(*m_Script, *currentGameRunner, *currentIndividual);

            threadController.AddIndividualRun(IndividualRun{
                currentIndividual,
                currentGameRunner,
                currentScriptRunner
            });
        }

        std::vector<IndividualRunResult> gameplayResults = threadController.ExecuteRuns();


        m_Status.m_ExecutionInfo.Stage = EAStage::WAITING_GENERATION;
        m_GuiProps.MainGameRunner = nullptr;

        //TODO: send m_EvolutionInfo to EvolutionaryAlgorithm.cpp to get more info, such as best and worst fitness, etc...
        m_Status.m_EvolutionInfo.CurrentGeneration++;


        usleep(5e2);

        return gameplayResults;
    }

} // namespace EvoAlg