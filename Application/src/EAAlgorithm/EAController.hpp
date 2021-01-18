#pragma once

#include "EAAlgorithm/EvolutionaryAlgorithm.hpp"
#include "Application/Game/GameRunner.hpp"


using namespace Application;

namespace EAAlgorithm
{

    class EAScript;
    class EvolutionaryAlgorithm;

    class EAController
    {
    public:
        //TODO: move concepts to its own file
        using Individual = std::vector<double>;
        using TimeResult = std::pair<double, double>;
        using GameplayResult = std::vector<TimeResult>;

    private:
        GameRunner &m_MainGameRunner;
        EvolutionaryAlgorithm m_Algorithm;
        std::vector<GameplayResult> m_GameplayResults;


    public:
        EAController(GameRunner &mainGameRunner);
        
        void StartEA();
        

        //DO NOT CALL ASYNCHRONOUSLY
        std::vector<GameplayResult> RunPopulationInGame(const std::vector<Individual> &population);
        
    };
} // namespace EAAlgorithm
