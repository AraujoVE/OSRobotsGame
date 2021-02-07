#pragma once

#include "Application/Events/EventHandler/EventHandler.template.hpp"

#define DA_EVENT_FWD(name, ...) class name;

namespace EvoAlg {
    struct EAGuiProps;
}


namespace Application
{
    

    /*  ROBOTS MANAGEMENT EVENT HANDLERS  */
    DA_EVENT_FWD(EH_TaskCreated, bool, (Task&));
    DA_EVENT_FWD(EH_TaskStarted, bool, (Task&));
    DA_EVENT_FWD(EH_TaskCancelled, bool, (Task&));
    DA_EVENT_FWD(EH_TaskFinished, bool, (Task&));
    DA_EVENT_FWD(EH_RobotsCreated, bool, (int));
    DA_EVENT_FWD(EH_RobotsDestroyed, bool, (int));
    DA_EVENT_FWD(EH_RobotsMoved, bool, (Task&, int));

    /* VILLAGE STATS EVENT HANDLERS */
    DA_EVENT_FWD(EH_DecaymentStopped, bool, (void));

    /* GAME RUNNER EVENT HANDLERS */
    DA_EVENT_FWD(EH_GameStarted, bool, (GameRunner&));
    DA_EVENT_FWD(EH_GameEnded, bool, (GameRunner&,int));
    

    /* THREADLOOP EVENTS */
    DA_EVENT_FWD(EH_ThreadStarted, bool, (void));
    DA_EVENT_FWD(EH_ThreadEnded, bool, (ThreadEndedReason::ThreadEndedReason_t));

    /* EAGAME GUI LAYER EVENTS */
    DA_EVENT_FWD(EH_EAGuiPropsChanged, bool, (const EvoAlg::EAGuiProps&));

    /* GAME CONSTS EVENTS */
    DA_EVENT_FWD(EH_GameConstsChanged, bool, ());

    //TODO: remove
    /* EA HANDLERS */
    DA_EVENT_FWD(EH_EAScriptEnded, bool, (int /* individualIndex */,  std::vector<std::pair<double, double>> /* gameplayResults */));

    
} // namespace Application
