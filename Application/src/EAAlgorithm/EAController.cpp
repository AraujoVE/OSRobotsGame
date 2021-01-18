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

    std::vector<EAController::GameplayResult> EAController::RunPopulationInGame(const std::vector<Individual> &population)
    {
        m_GameplayResults.clear();

        std::vector<EAScript *> scripts;

        for (const Individual &individual : population)
        {
            GameRunner indvGameRunner;
            indvGameRunner.GetGameConsts().LoadFromCromossome(individual);
            EAScript *script = new EAScript(indvGameRunner, getEAScriptSrcFilePath());
            script->startScript();

            scripts.push_back(script);
        }

        for (auto *script : scripts)
        {
            m_GameplayResults.push_back(*script->joinScriptThread());
        }

        return m_GameplayResults;
    }

} // namespace EAAlgorithm