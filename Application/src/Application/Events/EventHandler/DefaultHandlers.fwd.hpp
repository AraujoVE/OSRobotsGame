#pragma once

#include "Application/Events/EventHandler/EventHandler.template.hpp"

#define DA_EVENT_FWD(name, ...) class name;

namespace Application
{
    struct EAGameSettings;

    /*  ROBOTS MANAGEMENT EVENT HANDLERS  */
    DA_EVENT_FWD(EH_TaskCreated, bool, (Task&));
    DA_EVENT_FWD(EH_TaskStarted, bool, (Task&));
    DA_EVENT_FWD(EH_TaskCancelled, bool, (Task&));
    DA_EVENT_FWD(EH_TaskFinished, bool, (Task&));
    DA_EVENT_FWD(EH_RobotsCreated, bool, (int));
    DA_EVENT_FWD(EH_RobotsDestroyed, bool, (int));
    DA_EVENT_FWD(EH_RobotsMoved, bool, (Task&, int));

    /* VILLAGE STATS EVENT HANDLERS */
    DA_EVENT_FWD(EH_StatsDecayed, bool, (void));

    /* GAME RUNNER EVENT HANDLERS */
    DA_EVENT_FWD(EH_GameStarted, bool, (GameRunner&));
    DA_EVENT_FWD(EH_GameEnded, bool, (GameRunner&));
    

    /* THREADLOOP EVENTS */
    DA_EVENT_FWD(EH_ThreadStarted, bool, (void));
    DA_EVENT_FWD(EH_ThreadEnded, bool, (ThreadEndedReason::ThreadEndedReason_t));

    /* EAGAME GUI LAYER EVENTS */
    DA_EVENT_FWD(EH_EAGameSettingsChanged, bool, (EAGameSettings));

    /* GAME CONSTS EVENTS */
    DA_EVENT_FWD(EH_GCValueChanged, bool, (const std::string& /* changed value key */, float /* changed value */));


    //TODO: remove
    /* EA HANDLERS */
    DA_EVENT_FWD(EH_EAScriptEnded, bool, (int /* individualIndex */,  std::vector<std::pair<double, double>> /* gameplayResults */));

    
} // namespace Application
