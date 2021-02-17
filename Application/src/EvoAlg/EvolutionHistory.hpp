#pragma once

#include "Types.hpp"

#include <cstdint>
#include <vector>
#include <memory>

namespace EvoAlg
{
    struct GenerationFitnessInfo {
        double BestIndvFitness, worstIndvFitness, averageIndvFitness;
    };

    struct GenerationResult
    {
        //TODO: remove debug limitation
        GenerationResult(const GenerationResult&) = delete;
        uint64_t GenerationID;
        GenerationFitnessInfo FitnessInfo;
        GeneVec BestIndvGenes;
    };

    class EvolutionHistory
    {
        std::vector<GenerationResult> generationsResults;

    public:
        EvolutionHistory() {}
        void OnGenerationCompleted(uint64_t generationID, GenerationFitnessInfo fitnessInfo, const GeneVec& bestIndvGenes) {
            generationsResults.push_back({generationID, fitnessInfo, bestIndvGenes});
        }
    };

} // namespace EvoAlg