#pragma once

// #define DA_FORWARD 1
namespace Application
{

    class EH_TaskCreated;
    class EH_TaskEnded;
    class EH_RobotsCreated;
    class EH_RobotsDestroyed;
    class EH_RobotsMoved;

    /* VILLAGE STATS EVENT HANDLERS */
    class EH_StatsDecayed;

    /* GAME RUNNER EVENT HANDLERS */
    class EH_GameStarted;
    class EH_GameEnded;

} // namespace Application

// #undef DA_FORWARD
// #define DA_FORWARD 0