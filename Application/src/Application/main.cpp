#include "mypch.hpp"
#include <DampEngine/Core/EntryPoint.hpp>

#include "Application/ImGui/GameGuiLayer.hpp"
#include "Application/header/ConstsMap.hpp"

namespace Application
{
    class MyApplication : public DampEngine::Application
    {
    private:
        MyApplication *s_Instance = nullptr;
    public:
        MyApplication() : DampEngine::Application({"OSRobotGame"})
        {
            
        }

        virtual void OnStart() override
        {
            DE_INFO("OSRobotsGame starting");
            ConstsMap::initMapFromFile("constValues.cfg");
            m_GameSave.reset(new GameSave());
        }

        virtual void InitLayers() override
        {
            m_MainGuiLayer = new GameGuiLayer(*m_GameSave.get());
            m_LayerStack.PushOverlay(m_MainGuiLayer);
        }

        virtual void OnUpdate() override
        { 
        }

        virtual void OnStop() override
        {
            DE_INFO("OSRobotsGame stopping");
        }
        
    private:
        std::unique_ptr<GameSave> m_GameSave;
        GameGuiLayer *m_MainGuiLayer;
    };

} // namespace Application

DampEngine::Application *CreateApplication()
{
    return new Application::MyApplication();
}
