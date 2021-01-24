#pragma once

#include "EvoAlg/EvolutionaryAlgorithm.hpp"

#include "EvoAlg/Types.hpp"
#include "EvoAlg/Script.hpp"
#include "EvoAlg/EAStatus.hpp"

#include "Application/Game/GameRunner.hpp"

using namespace Application;

namespace EvoAlg
{
    class ScriptRunner;
    class EvolutionaryAlgorithm;

    class EAController
    {
    public:
        //TODO: move concepts to its own file
        

    private:
        EvolutionaryAlgorithm m_Algorithm;

        
        //TODO: unique_ptr, or delete
        Script *m_Script;

        EAStatus m_Status;
        EAGuiProps &m_GuiProps;

    public:
        EAController(EAGuiProps& guiProps);
        
        void Start();
        void Cancel();

        inline bool IsRunning() { return m_Status.CurrentStage != EAStage::INACTIVE; }
        

        //DO NOT CALL ASYNCHRONOUSLY
        std::vector<IndividualRunResult> RunPopulationInGame(const std::vector<GeneVec> &population);
        
    };
} // namespace EAAlgorithm
