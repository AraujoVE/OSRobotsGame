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
        

        // m_Algorithm.startAlgorithm();

        Application::Action<> rpig([this] {
            RunPopulationInGame({});
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
        GameConsts *fileGameConsts = new GameConsts();
        fileGameConsts->LoadValuesFromFile(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_CONSTS));

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

        // GameConsts *gameConsts = new GameConsts();
        // gameConsts->SetTickDelay(1);
        auto *aaa = new GameRunner(fileGameConsts);
        m_GuiProps.MainGameRunner = aaa;

        //TODO: event EH_GameAttached (wait for UI to be ready)
        usleep(5e6);

        DE_INFO("(EAController) Preparing population to be executed...");
        for (unsigned int i = 0; i < 20; i++)
        {
            DE_INFO("(EAController) Preparing individual #{0}", i);

            DE_INFO("(EAController) Copying genes individual #{0}", i);
            Individual *currentIndividual = new Individual{i, {}};

            DE_INFO("(EAController) Creating game runner for individual #{0}", i);
            GameRunner *currentGameRunner = aaa;

            DE_DEBUG("(RunPopulationInGame) Loading game runner with cromossome...");
            // currentGameRunner->GetGameConsts().LoadFromCromossome(populationGenes[i]);

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
            break;
        }

        //TODO: free all scripts and gameRunners

        m_Status.m_ExecutionInfo.Stage = EAStage::WAITING_GENERATION;
        m_GuiProps.MainGameRunner = nullptr;

        DE_DEBUG("(RunPopulationInGame) All individuals have been tested! returning results");
        return gameplayResults;
    }

} // namespace EvoAlg