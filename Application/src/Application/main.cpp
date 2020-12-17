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

        virtual void OnStart() override
        {
        }

        virtual void InitLayers() override
        {
            m_LayerStack.PushOverlay(new MainGuiLayer(m_GameSave));
        }

        virtual void OnUpdate() override
        { 
            DE_DEBUG("Fora de UI, Population = {0}", m_GameSave.getVillageStats().getPopulation());
        }

        virtual void OnStop() override
        {
        }

    private:
        GameSave m_GameSave;
    };

} // namespace Application

DampEngine::Application *CreateApplication()
{
    return new Application::MyApplication();
}