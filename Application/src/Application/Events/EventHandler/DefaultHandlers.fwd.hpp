#pragma once

#include "Application/Events/EventHandler/EventHandler.template.hpp"

#define DA_EVENT_FWD(name, ...) class name;

namespace EvoAlg {
    struct EAGuiProps;
}


namespace Application
{
    /*  ROBOTS MANAGEMENT EVENT HANDLERS  */
    DA_EVENT_FWD(EH_TaskCreated, bool, Task &);
    DA_EVENT_FWD(EH_TaskStarted, bool, Task &);
    DA_EVENT_FWD(EH_TaskCancelled, bool, Task &);
    DA_EVENT_FWD(EH_TaskFinished, bool, Task &);
    DA_EVENT_FWD(EH_RobotsCreated, bool, int);
    DA_EVENT_FWD(EH_RobotsDestroyed, bool, int);
    DA_EVENT_FWD(EH_RobotsMoved, bool, Task &, int);

    /* VILLAGE STATS EVENT HANDLERS */
    DA_EVENT_FWD(EH_DecaymentStarted, bool);
    DA_EVENT_FWD(EH_DecaymentStopped, bool);

    /* GAME RUNNER EVENT HANDLERS */
    DA_EVENT_FWD(EH_GameStarted, bool, GameRunner &);
    DA_EVENT_FWD(EH_GameEnded, bool, GameRunner &, int);

    /* THREADLOOP EVENTS */
    DA_EVENT_FWD(EH_ThreadStarted, bool);
    DA_EVENT_FWD(EH_ThreadEnded, bool, ThreadEndedReason::ThreadEndedReason_t);

    /* GAME CONSTS EVENTS */
    DA_EVENT_FWD(EH_GameConstsChanged, bool);

    

} // namespace Application


namespace EvoAlg {
    //TODO?: remove
    /* EAGAME GUI LAYER EVENTS */
    DA_EVENT_FWD(EH_GuiPropsChanged, bool, const EvoAlg::EAGuiProps &);

    /* EA HANDLERS */
    DA_EVENT_FWD(EH_ScriptEnded, bool, int /* individualIndex */, std::vector<std::pair<double, double>> /* gameplayResults */);
    DA_EVENT_FWD(EH_GameRunnerChanged, bool, EvoAlg::ThreadID, GameRunner* /* new game runner, nullable*/);
}