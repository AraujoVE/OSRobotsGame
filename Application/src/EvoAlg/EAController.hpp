#pragma once

#include "EvoAlg/EvolutionaryAlgorithm.hpp"
#include "EvoAlg/Types.hpp"

#include "Application/Game/GameRunner.hpp"
#include "Script.hpp"

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

        //Index of array corresponds to IndividualID
        std::vector<GameplayResult> m_GameplayResults;
        
        //TODO: unique_ptr, or delete
        Script *m_Script;
        


    public:
        EAController();
        
        void StartEA();
        

        //DO NOT CALL ASYNCHRONOUSLY
        std::vector<GameplayResult> RunPopulationInGame(const std::vector<GeneVec> &population);
        
    };
} // namespace EAAlgorithm
