#pragma once

#include <vector>

namespace EvoAlg
{
    using GeneVec = std::vector<double>;
    
    struct Individual {
        unsigned int ID;
        GeneVec Genes;
    };

    struct TimeResult {
        double TargetTime, MeasuredTime;
    };
    
    //Index of vector corresponds to gameplay ID, which is in range [0, inf[
    using GameplayResult = std::vector<TimeResult>;

} // namespace EvoAlg