#pragma once

#include "Application/ImGui/IGWindow.hpp"
#include "Application/Game/Ingame/Task.hpp"

#include "DampEngine/Threads/Mutex.hpp"

#include <queue>

namespace Application
{
    class RobotsManagement;

    namespace GameWindows
    {
        class TaskWindow;

        class FunctionWindow final : public Application::IGWindow
        {
        public:
            FunctionWindow(RobotsManagement *robotsManagement, RobotFunction function);

            virtual void Render() override;

            void OnTaskEnded(Task &endedTask);
            void OnTaskEnded(int id);

            void ClearTaskWindows();

            virtual void SetEventHandlers(RobotsManagement *robotManagement);

        private:
        private:

            DampEngine::Mutex m_TaskWindowMapMutex, m_TaskDeletionQueueMutex;

            RobotFunction m_Function;
            RobotsManagement* m_RobotsManagement;
            std::unordered_map<Task::TaskID, TaskWindow *> m_TaskWindowMap;
            std::queue<Task::TaskID> m_TasksPendingDeletion;
        };
    } // namespace GameWindows

} // namespace Application