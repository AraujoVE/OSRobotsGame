#include "EAController.hpp"

#include "DampEngine/Core/Macros/Log.hpp"
#include "EAAlgorithm/ConvertScripts.hpp"

#include "EAAlgorithm/EAScript.hpp"
#include "Application/header/ConstsMap.hpp"
#include "mypch.hpp"

//TODO: move to Util
static std::string getCWD()
{
    char path[1024];
    getcwd(path, 1024);
    std::string cwd(path);
    return cwd;
}

//TODO: move to Util
static std::string getEAScriptSrcFilePath()
{
    return getCWD() + "/gameScript.cfg";
}

static void convertScripts()
{
    DE_TRACE("Converting all scripts in {0}/scripts", std::string(getCWD()));
    EAAlgorithm::ConvertScripts convertScripts(getCWD());
}

namespace EAAlgorithm
{
    EAController::EAController(GameRunner &mainGameRunner) : m_MainGameRunner(mainGameRunner), m_Algorithm(*this)
    {
    }

    void EAController::StartEA()
    {
        convertScripts();
        m_Algorithm.startAlgorithm();
    }

    //TODO: Sync or Async UI
    std::vector<EAController::GameplayResult> EAController::RunPopulationInGame(const std::vector<Individual> &population)
    {
        DE_DEBUG("EAController::RunPopulationInGame()");
        m_GameplayResults.clear();

        std::vector<EAScript *> scripts;
        std::vector<GameRunner *> gameRunners;

        //TODO: fix ml
        //TODO: DELAY MCRO from UI
        //Used only to get DELAY_MICRO
        GameConsts *fileGC = new GameConsts();
        fileGC->LoadValuesFromFile(getEAScriptSrcFilePath());


        int debug_ind_ind = 0;
        for (const Individual &individual : population)
        {
            //Feeds indvGameRunner with base consts from file, before overwriting with genes
            GameRunner *indvGameRunner = new GameRunner();

            DE_DEBUG("(RunPopulationInGame) Loading game runner with cromossome...");
            indvGameRunner->GetGameConsts().LoadFromCromossome(individual);
            DE_DEBUG("(RunPopulationInGame) Done.");
            EAScript *script = new EAScript(*indvGameRunner, getEAScriptSrcFilePath(), std::to_string(debug_ind_ind));

            DE_DEBUG("(RunPopulationInGame) Executing (async) gameScript.cfg on individual {0}...", ++debug_ind_ind);
            script->startScript();

            scripts.push_back(script);
            gameRunners.push_back(indvGameRunner);
            break;
        }

        debug_ind_ind = 0;
        for (auto *script : scripts)
        {
            DE_DEBUG("(RunPopulationInGame) Waiting for script to end (script of individual {0})...", ++debug_ind_ind);
            m_GameplayResults.push_back(*script->joinScriptThread());
        }

        DE_DEBUG("(RunPopulationInGame) All individuals have been tested! returning results");
        return m_GameplayResults;
    }

} // namespace EAAlgorithm