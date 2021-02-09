#pragma once

#include "Application/Game/Types.fwd.hpp"
#include <vector>


namespace EvoAlg
{
    using ThreadID = uint64_t;
    using GeneVec = std::vector<double>;
    
    struct Individual {
        unsigned int ID;
        const GeneVec& Genes;
    };

    struct TimeResult {
        double TargetTime, MeasuredTime;
    };
    
    //Index of vector corresponds to gameplay ID, which is in range [0, inf[
    using IndividualRunResult = std::vector<TimeResult>;


    struct EAGuiProps
    {
        bool
            ShowGame = false,
            Pause = false;

        Application::GameRunner *MainGameRunner = nullptr;
    };


} // namespace EvoAlg