#include "DampEngine/ImGui/ImGuiLayer.hpp"
using namespace DampEngine;

#include "Windows/FunctionWindow.hpp"
#include "header/Task.hpp"
#include <memory>

class MainGuiLayer final : public ImGuiLayer
{
public:
    MainGuiLayer();

private:
    virtual void ImGuiDescription() override;

private:
    std::unique_ptr<Task> m_Task;

    std::unique_ptr<FunctionWindow> m_FunctionWindow;
};