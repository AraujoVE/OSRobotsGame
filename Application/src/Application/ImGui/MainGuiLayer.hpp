#include "DampEngine/ImGui/ImGuiLayer.hpp"
using namespace DampEngine;

#include "Application/ImGui/Windows/FunctionWindow.hpp"
#include "Application/header/Task.hpp"
#include <memory>
namespace Application
{
    class MainGuiLayer final : public ImGuiLayer
    {
    public:
        MainGuiLayer();

    private:
        virtual void ImGuiDescription() override;

    private:
        std::unique_ptr<FunctionWindow> m_FunctionWindow;
    };
}