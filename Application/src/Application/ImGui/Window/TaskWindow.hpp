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

        unsigned FunctionTaskVecIdx;
        IGWindowProps ParentProps;
        unsigned xOffsetFirst, xSpacing;

        TaskWindowProps(unsigned functionTaskVecIdx, IGWindowProps parentProps, unsigned xOffsetFirst = 10, unsigned xSpacing = 10)
            : FunctionTaskVecIdx(functionTaskVecIdx), ParentProps(parentProps), xOffsetFirst(xOffsetFirst), xSpacing(xSpacing)
        {
        }
    };

    class TaskWindow final : public Application::IGWindow
    {
    public:
        using OnTaskCancelledFn = std::function<void(TaskWindow *)>;
        TaskWindow(TaskWindowProps taskWindowProps, RobotsManagement &robotsManagement, TaskID taskID, OnTaskCancelledFn onTaskCancelledFn);

        inline TaskID GetTaskID() const { return m_TaskID; }
        inline void SetIndex(int newIndex)
        {
            m_TaskWindowProps.FunctionTaskVecIdx = newIndex;
            UpdateProps();
        }

        virtual void Render() override;

    private:
        void UpdateProps();

    private:
        TaskWindowProps m_TaskWindowProps;
        RobotsManagement &m_RobotsManagement;
        TaskID m_TaskID;
        std::string m_WindowName;
        OnTaskCancelledFn m_OnTaskCancelledFn;
    };
} // namespace Application