#include "mypch.hpp"
#include <DampEngine/Core/EntryPoint.hpp>

#include "Application/ImGui/GameGuiLayer.hpp"
#include "Application/header/ConstsMap.hpp"

#include <filesystem>

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
            auto cwd =  std::filesystem::current_path();
            ConstsMap::initMapFromFile(cwd.concat("/Application/config/constValues.cfg"));
        }

        virtual void InitLayers() override
        {
            m_MainGuiLayer = new GameGuiLayer();
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
        GameGuiLayer *m_MainGuiLayer;
    };

} // namespace Application

DampEngine::Application *CreateApplication()
{
    return new Application::MyApplication();
}
