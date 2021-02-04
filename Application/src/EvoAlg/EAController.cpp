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

        // ThreadController threadController;

        //TODO: change
        m_GuiProps.MainGameRunner = nullptr;

        //TODO: event EH_GGuiGameAttached (wait for UI to be ready (only if ShowGame is true))
        // usleep(1);

        std::vector<void *> pointersPendingDeletion;

        ScriptRunner scriptRunner(*m_Script);

        // for (unsigned int i = 0; i < populationGenes.size(); i++)
        // {
        //     threadController.AddIndividual(Individual{i, populationGenes[i]});
        // }

        // std::vector<IndividualRunResult> gameplayResults = threadController.ExecuteAllIndividuals(scriptRunner);

        //TODO: REMOVE TEMP TEST AFTER PRESENTATION
        static bool first = true;
        static GameConsts *gameConsts = new GameConsts();
        static GameRunner *singleThreadGR = new GameRunner(gameConsts);
        if (first) { gameConsts->SetTickDelay(50e3); first = false; } 
        m_GuiProps.MainGameRunner = singleThreadGR;


        std::vector<IndividualRunResult> gameplayResults;
        gameplayResults.reserve(populationGenes.size());

        for (unsigned int i = 0; i < populationGenes.size(); i++)
        {
            Individual indv{i, populationGenes[i]};
            gameConsts->LoadFromCromossome(populationGenes[i]);
            gameplayResults.push_back(*scriptRunner.RunAllGameplays(*singleThreadGR, indv));
            DE_TRACE("Indo pro próximo individuo.");
        }

        m_Status.m_ExecutionInfo.Stage = EAStage::WAITING_GENERATION;

        //TODO: send m_EvolutionInfo to EvolutionaryAlgorithm.cpp to get more info, such as best and worst fitness, etc...
        m_Status.m_EvolutionInfo.CurrentGeneration++;

        usleep(5e2);

        return gameplayResults;
    }

} // namespace EvoAlg