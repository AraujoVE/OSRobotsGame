#pragma once

#include "Application/ImGui/IGWindow.hpp"
#include "Application/header/Task.hpp"
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
            FunctionWindow(std::unique_ptr<Application::RobotsManagement> &robotsManagement, RobotFunction function);

            virtual void Render() override;

            void OnTaskEnded(Task &endedTask);
            void OnTaskEnded(int id);

            void ClearTaskWindows();
            TaskWindow *getTaskWindow(TaskID id);

            virtual void SetEventHandlers(std::unique_ptr<Application::RobotsManagement> &robotManagement);

        private:
        private:
            RobotFunction m_Function;
            std::unique_ptr<Application::RobotsManagement> &m_RobotsManagement;
            std::unordered_map<TaskID, TaskWindow *> m_TaskWindowMap;
            std::queue<TaskID> m_TasksPendingDeletion;
            pthread_mutex_t m_MutexMapRemoval;
        };
    } // namespace GameWindows

} // namespace Application