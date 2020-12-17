#include "DampEngine/ImGui/ImGuiLayer.hpp"
using namespace DampEngine;

#include "Application/ImGui/Window/FunctionWindow.hpp"

#include "Application/header/Task.hpp"
#include "Application/header/GameSave.hpp"
#include "Application/header/RobotFunctions.hpp"

#include <vector>
#include <functional>

namespace Application
{
    class MainGuiLayer final : public ImGuiLayer
    {
    public:
        MainGuiLayer(GameSave &gameSave);

    private:
        virtual void ImGuiDescription() override;

    private:
        GameSave &m_GameSave;
        FunctionWindow* m_FunctionWindows[RobotsManagement::FUNCTION_QTY];
    };
} // namespace Application