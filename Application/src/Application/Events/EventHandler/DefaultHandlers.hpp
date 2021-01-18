#pragma once

#include "DefaultHandlers.fwd.hpp"

#include "Application/Events/EventHandler/EventHandler.template.hpp"

#include "Application/Game/GameRunner.hpp"
#include "Application/header/Task.hpp"
#include <string>

namespace Application
{
    /*  ROBOTS MANAGEMENT EVENT HANDLERS  */
    DA_EVENT(EH_TaskCreated, bool, (Task&));
    DA_EVENT(EH_TaskStarted, bool, (Task&));
    DA_EVENT(EH_TaskCancelled, bool, (Task&));
    DA_EVENT(EH_TaskFinished, bool, (Task&));
    DA_EVENT(EH_RobotsCreated, bool, (int));
    DA_EVENT(EH_RobotsDestroyed, bool, (int));
    DA_EVENT(EH_RobotsMoved, bool, (Task&, int));

    /* VILLAGE STATS EVENT HANDLERS */
    DA_EVENT(EH_StatsDecayed, bool, (void));

    /* GAME RUNNER EVENT HANDLERS */
    DA_EVENT(EH_GameStarted, bool, (GameRunner&));
    DA_EVENT(EH_GameEnded, bool, (GameRunner&));
    

    /* THREADLOOP EVENTS */
    DA_EVENT(EH_ThreadStarted, bool, (void));
    DA_EVENT(EH_ThreadEnded, bool, (ThreadEndedReason::ThreadEndedReason_t));

    /* EAGAME GUI LAYER EVENTS */
    DA_EVENT(EH_EAGameSettingsChanged, bool, (EAGameSettings));

    /* GAME CONSTS EVENTS */
    DA_EVENT(EH_GCValueChanged, bool, (const std::string& /* changed value key */, float /* changed value */));

    
} // namespace Application
