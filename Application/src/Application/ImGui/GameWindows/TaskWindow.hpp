#pragma once

#include "Application/ImGui/IGWindow.hpp"
#include "Application/Game/Ingame/Task.hpp"

namespace Application
{
    class RobotsManagement;

    namespace GameWindows
    {

        struct TaskWindowProps
        {

            unsigned long FunctionTaskVecIdx;
            IGWindowProps ParentProps;
            unsigned xOffsetFirst, xSpacing;

            TaskWindowProps(unsigned long functionTaskVecIdx, IGWindowProps parentProps, unsigned xOffsetFirst = 10, unsigned xSpacing = 10)
                : FunctionTaskVecIdx(functionTaskVecIdx), ParentProps(parentProps), xOffsetFirst(xOffsetFirst), xSpacing(xSpacing)
            {
            }
        };

        class TaskWindow final : public Application::IGWindow
        {
        public:
            //When user choses to close current Task, this callback is called (you must destroy the TaskWindow associated object in the callback)
            using OnTaskCancelledFn = std::function<void(Task &)>;

            TaskWindow(
                TaskWindowProps taskWindowProps,
                RobotsManagement* robotsManagement,
                Task &task,
                OnTaskCancelledFn onTaskCancelledFn);

            ~TaskWindow()
            {
            }

            inline Task &GetTask() const { return m_Task; }
            inline void SetIndex(unsigned long newIndex)
            {
                m_TaskWindowProps.FunctionTaskVecIdx = newIndex;
                UpdateProps();
            }

            inline unsigned long GetIndex() const { return m_TaskWindowProps.FunctionTaskVecIdx; }

            virtual void Render() override;

        private:
            void UpdateProps();

        private:
            TaskWindowProps m_TaskWindowProps;
            RobotsManagement* m_RobotsManagement;
            Task &m_Task;
            std::string m_WindowName;
            OnTaskCancelledFn m_OnTaskCancelledFn;
        };
    } // namespace GameWindows
} // namespace Application