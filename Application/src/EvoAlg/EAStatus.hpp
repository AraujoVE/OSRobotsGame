#pragma once

namespace EvoAlg
{
    //TODO: remove if end up not being used
    enum class EAMode
    {
        PRESENTATION,
        SPEED
    };

    enum class EAStage
    {
        INACTIVE = 0,
        WAITING_GENERATION,
        RUNNING_GENERATION,
        FINISHED,
        ABORTED
    };


    struct EvolutionInfo {
        uint64_t CurrentGeneration = 0; //0 means not started yet
    };

    struct ExecutionInfo {
        EAStage Stage = EAStage::INACTIVE;
        EAMode Mode = EAMode::PRESENTATION;
        uint8_t MaxThreads = 1;
    };

    struct EAStatus
    {

        /* EASTATUS AND EAPROPERTIES : 2 CLASSES
    
        * MODES:
            - SPEED -> A LOT OF THREADS WITH NO GAME VISUALISATION
            - PRESENTATION -> ONE SINGLE THREAD, SHOWING IN SCREEN AS A GAME

        * MAX NUMBER OF THREADS WITH SPEED MODE

        * EXECUTION SPEED: PROBABLY IN OTHER CLASS MORE RELATED TO THE GAME RUNNER 

        PS: INDEPENDENTLY OF THE CHOSEN MODE, THE UI WILL SHOW A LIST OF ALL EXECUTING THREADS (ONE PER INDIVIDUAL), AND ITS PROPERTIES, SUCH AS POPULATION, TOTROBOTS, EXECUTIONTIME (IN TICKS), ETC.

    */

        ExecutionInfo m_ExecutionInfo;
        EvolutionInfo m_EvolutionInfo;
    };

} // namespace EvoAlg