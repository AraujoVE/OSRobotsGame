#include "Application/Game/GameConsts.hpp"

#include "EvoAlg/EAController.hpp"

#include "EvoAlg/ScriptConverter.hpp"
#include "EvoAlg/ScriptRunner.hpp"

#include "Application/Util/path.hpp"

#include "mypch.hpp"

#include "EvoAlg/Types.hpp"

namespace EvoAlg
{
    EAController::EAController() : m_Algorithm(*this), m_Script(nullptr)
    {
    }

    void EAController::StartEA()
    {
        ScriptConverter converter(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_SCRIPT_HUMAN_FOLDER));
        m_Script = converter.Convert();

        //m_Script.Save()

        m_Algorithm.startAlgorithm();
    }

    //TODO: Sync or Async UI
    //TODO: assert is being called from only one thread (maybe mutex)
    std::vector<GameplayResult> EAController::RunPopulationInGame(const std::vector<GeneVec> &populationGenes)
    {
        DE_ASSERT(m_Script != nullptr);

        DE_DEBUG("EAController::RunPopulationInGame()");
        //TODO: maybe local variable instead of class member
        m_GameplayResults.clear();

        //TODO: use this struct in a vector instead of 2 vecs below
        struct IndividualRun
        {
            Individual *individual;
            GameRunner *gameRunner;
            ScriptRunner *scriptRunner;
        };

        std::vector<IndividualRun *> gameRuns;

        //TODO: DELAY MCRO from UI

        // GameRunner *globalDeletemeGR = new GameRunner();

        DE_INFO("(EAController) Preparing population to be executed...");
        for (unsigned int i = 0; i < populationGenes.size(); i++)
        {
            DE_INFO("(EAController) Preparing individual #{0}", i);

            DE_INFO("(EAController) Copying genes individual #{0}", i);
            Individual *currentIndividual = new Individual{i, populationGenes[i]};

            DE_INFO("(EAController) Creating game runner for individual #{0}", i);
            GameRunner *currentGameRunner = new GameRunner();
            
            DE_DEBUG("(RunPopulationInGame) Loading game runner with cromossome...");
            currentGameRunner->GetGameConsts().LoadFromCromossome(populationGenes[i]);

            DE_DEBUG("(RunPopulationInGame) Loaded Successfully.");

            gameRuns.push_back(new IndividualRun{
                currentIndividual,
                currentGameRunner,
                new ScriptRunner(*m_Script, *currentGameRunner, *currentIndividual)});
        }

        // bool syncExecution = false;
        DE_INFO("(EAController) Executing all individuals");
        for (auto &currentRun : gameRuns)
        {
            DE_DEBUG("(EAController) Executing script on individual {0}...", currentRun->individual->ID);
            GameplayResult *result = (GameplayResult *)currentRun->scriptRunner->scriptLoop();
            m_GameplayResults.push_back(*result);

            //TODO: allow UI to execute synchronously
            // if (syncExecution) script->*script->joinScriptThread();
            // change UI to next gameRunner
        }

        // //TODO: remove, using vec of struct above
        // int debug_ind_ind = 0;
        // for (auto &currentRun : gameRuns)
        // {
        //     DE_DEBUG("(RunPopulationInGame) Waiting for script to end (script of individual {0})...", ++debug_ind_ind);
        //     //TODO: fix ml
        //     GameplayResult *result = currentRun->scriptRunner->joinScriptThread();

        //     m_GameplayResults.push_back(*result);
        // }

        //TODO: free all scripts and gameRunners

        DE_DEBUG("(RunPopulationInGame) All individuals have been tested! returning results");
        return m_GameplayResults;
    }

} // namespace EvoAlg