#pragma once

namespace EvoAlg
{

    enum class EAMode
    {
        PRESENTATION,
        SPEED
    };

    enum class EAStage
    {
        INACTIVE,
        WAITING_GENERATION,
        RUNNING_GENERATION,
        ABORTED
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

        unsigned char MaxThreads = 1;
        EAMode SelectedMode = EAMode::SPEED;
        EAStage CurrentStage = EAStage::INACTIVE;
    };

} // namespace EvoAlg