#include <DampEngine.hpp>

#include "ImGui/MainGuiLayer.hpp"

namespace Application
{
    class MyApplication : public DampEngine::Application
    {
    public:
        MyApplication() : DampEngine::Application({"OSRobotGame"})
        {
        }

        virtual void InitLayers() override
        {
            m_LayerStack.PushOverlay(new MainGuiLayer());
        }

        virtual void OnUpdate() override
        {
        }
    };


} // namespace Application

DampEngine::Application *CreateApplication()
{
    return new Application::MyApplication();
}