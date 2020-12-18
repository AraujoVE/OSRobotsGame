#pragma once

#include "IGWindow.hpp"

#include "imgui.h"

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include "Application/header/Task.hpp"

#include "Application/header/RobotsManagement.hpp"

namespace Application
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
        using OnTaskCancelledFn = std::function<void(TaskWindow *)>;

        TaskWindow(
            TaskWindowProps taskWindowProps,
            RobotsManagement &robotsManagement, 
            Task& task, 
            OnTaskCancelledFn onTaskCancelledFn
        );

        virtual ~TaskWindow() override 
        {
            //TODO: n fazer isso a cada frame kk
            m_RobotsManagement.endTask(m_Task, true);
        }
        
        inline TaskID GetTaskID() const { return m_Task.getId(); }
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
        RobotsManagement &m_RobotsManagement;
        Task& m_Task;
        std::string m_WindowName;
        OnTaskCancelledFn m_OnTaskCancelledFn;
    };
} // namespace Application