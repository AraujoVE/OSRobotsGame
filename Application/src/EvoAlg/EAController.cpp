#include "Application/Game/GameConsts.hpp"

#include "EvoAlg/EAController.hpp"

#include "EvoAlg/ScriptConverter.hpp"
#include "EvoAlg/ScriptRunner.hpp"

#include "Application/Util/path.hpp"

#include "mypch.hpp"

#include "EvoAlg/Types.hpp"




//TODO: ????
static void convertScripts()
{
    DE_TRACE("Converting all scripts in {0}/scripts", std::string(Util::Path::getCWD()));

    //TODO: better syntax (such as .Start())
    EvoAlg::ScriptConverter convertScripts; //DANGER: THIS CONSTRUCTOR IS CONVERTING ALL THE SCRIPTS (CHANGE WEIRD BEHAVIOUR)
}

namespace EvoAlg
{
    EAController::EAController() : m_Algorithm(*this)
    {
    }

    void EAController::StartEA()
    {
        convertScripts();
        m_Algorithm.startAlgorithm();
    }

    //TODO: Sync or Async UI
    //TODO: assert is being called from only one thread (maybe mutex)
    std::vector<GameplayResult> EAController::RunPopulationInGame(const std::vector<GeneVec> &populationGenes)
    {
        DE_DEBUG("EAController::RunPopulationInGame()");
        //TODO: maybe local variable instead of class member
        m_GameplayResults.clear();


        //TODO: use this struct in a vector instead of 2 vecs below
        // struct IndividualRun {
        //     Individual *individual;
        //     GameRunner *gameRunner;
        //     ScriptRunner *scriptRunner;
        // }        

        //TODO: fix ml
        std::vector<ScriptRunner *> scripts;
        std::vector<GameRunner *> gameRunners;
        std::vector<Individual *> individuals;

        //TODO: DELAY MCRO from UI
        
        DE_INFO("(EAController) Preparing population to be executed...");
        for (unsigned int i = 0; i <  populationGenes.size(); i++)
        {
            DE_INFO("(EAController) Preparing individual #{0}", i);

            individuals.push_back(new Individual{i, populationGenes[i]});

            DE_DEBUG("(EAController) Creating GameRunner");
            GameRunner *indvGameRunner = new GameRunner();


            DE_DEBUG("(RunPopulationInGame) Loading game runner with cromossome...");
            indvGameRunner->GetGameConsts().LoadFromCromossome(individuals[i]->Genes);
            DE_DEBUG("(RunPopulationInGame) Done.");

            //TODO: use Script class to avoid a LOT of file openings
            ScriptRunner *script = new ScriptRunner(*indvGameRunner, Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_SCRIPT_MACHINE), std::to_string(i));

            DE_DEBUG("(RunPopulationInGame) Executing (async) gameScript.cfg on individual {0}...", i);

            scripts.push_back(script);
            gameRunners.push_back(indvGameRunner);
        }

        // bool syncExecution = false;
        DE_INFO("(EAController) Executing all individuals");
        for (auto *script: scripts) {
            script->startScript();
            
            //TODO: allow UI to execute synchronously
            // if (syncExecution) script->*script->joinScriptThread();
            // change UI to next gameRunner
        }

        //TODO: remove, using vec of struct above
        int debug_ind_ind = 0;
        for (auto *script : scripts)
        {
            DE_DEBUG("(RunPopulationInGame) Waiting for script to end (script of individual {0})...", ++debug_ind_ind);
            m_GameplayResults.push_back(*script->joinScriptThread());
        }

        //TODO: free all scripts and gameRunners

        DE_DEBUG("(RunPopulationInGame) All individuals have been tested! returning results");
        return m_GameplayResults;
    }

} // namespace EAAlgorithm