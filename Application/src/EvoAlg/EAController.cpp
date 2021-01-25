#include "Application/Game/GameConsts.hpp"

#include "EvoAlg/EAController.hpp"

#include "EvoAlg/ScriptConverter.hpp"
#include "EvoAlg/ScriptRunner.hpp"

#include "Application/Util/path.hpp"

#include "Application/Threads/Action.hpp"

#include "mypch.hpp"

#include "EvoAlg/Types.hpp"

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
        std::vector<IndividualRunResult> gameplayResults;

        struct IndividualRun
        {
            Individual *individual;
            GameRunner *gameRunner;
            ScriptRunner *scriptRunner;
        };

        GameConsts *gameConsts = new GameConsts();
        gameConsts->SetTickDelay(5e2);
        auto *aaa = new GameRunner(gameConsts);
        m_GuiProps.MainGameRunner = aaa;

        //TODO: event EH_GameAttached (wait for UI to be ready (only if ShowGame is true))
        usleep(1);

        DE_INFO("(EAController) Preparing population to be executed...");
        for (unsigned int i = 0; i < populationGenes.size(); i++)
        {
            //TODO: think what to do in async mode (probably vector of GameRunners)
            m_GuiProps.MainGameRunner = aaa;
            
            DE_INFO("(EAController) Preparing individual #{0}", i);

            DE_INFO("(EAController) Copying genes from individual #{0}", i);
            Individual *currentIndividual = new Individual{i, {populationGenes[i]}};

            DE_INFO("(EAController) Creating game runner for individual #{0}", i);
            GameRunner *currentGameRunner = aaa;

            DE_DEBUG("(RunPopulationInGame) Loading game runner with cromossome...");
            currentGameRunner->GetGameConsts().LoadFromCromossome(populationGenes[i]);

            DE_DEBUG("(RunPopulationInGame) Loaded Successfully.");
            ScriptRunner *currentScriptRunner = new ScriptRunner(*m_Script, *currentGameRunner, *currentIndividual);

            // IndividualRun *run = new IndividualRun{
            //     currentIndividual,
            //     currentGameRunner,
            //     currentScriptRunner
            // };

            //TODO: allow async with MAX_THREADS control
            IndividualRunResult *result = currentScriptRunner->RunAllGameplays();
            gameplayResults.push_back(*result);
            //TODO: fix ml
            //delete result;

            DE_TRACE("(RunPopulationInGame) Individual #{0} completed successfully", i);

            //TODO: remove visual hint of change
            // {
            //     m_GuiProps.MainGameRunner = nullptr;
            //     usleep(5e6);
            //     m_GuiProps.MainGameRunner = aaa;
            // }
            
            m_GuiProps.MainGameRunner = nullptr;
        }

        //TODO: free all scripts and gameRunners
        delete aaa;

        m_Status.m_ExecutionInfo.Stage = EAStage::WAITING_GENERATION;
        m_GuiProps.MainGameRunner = nullptr;

        //TODO: send m_EvolutionInfo to EvolutionaryAlgorithm.cpp to get more info, such as best and worst fitness, etc...
        m_Status.m_EvolutionInfo.CurrentGeneration++;

        DE_DEBUG("(RunPopulationInGame) All individuals have been tested! returning results");
        // usleep(50);
        return gameplayResults;
    }

} // namespace EvoAlg