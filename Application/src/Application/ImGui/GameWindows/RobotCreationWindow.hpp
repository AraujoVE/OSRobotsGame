#pragma once

#include "Application/ImGui/IGWindow.hpp"


#include <memory>

namespace Application
{
    class RobotsManagement;

    class RobotCreationWindow final : Application::IGWindow
    {
    public:
        RobotCreationWindow(std::unique_ptr<RobotsManagement> &robotsManagement)
            : IGWindow(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse),
              m_RobotsManagement(robotsManagement)
        {
            m_WindowProps = { 
                {100, 650},
                {300, 200}
            };
        }

        virtual void Render() override;

    private:
        std::unique_ptr<RobotsManagement> &m_RobotsManagement;
    };
} // namespace Application
