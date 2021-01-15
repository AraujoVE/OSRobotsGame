#include "mypch.hpp"
#include <DampEngine/Core/EntryPoint.hpp>

#include "Application/ImGui/GameGuiLayer.hpp"
#include "Application/ImGui/EAGameGuiLayer.hpp"
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
            char path[1024];
            getcwd(path,1024);
            std::string cwd(path);
            //auto cwd =  std::filesystem::current_path();
            ConstsMap::initMapFromFile(cwd + "/Application/config/constValues.cfg");
        }

        virtual void InitLayers() override
        {
            m_GameGuiLayer = new GameGuiLayer();
            m_LayerStack.PushOverlay(m_GameGuiLayer);
            // m_EAGameGuiLayer = new EAGameGuiLayer();
            // m_LayerStack.PushOverlay(m_EAGameGuiLayer);
        }

        virtual void OnUpdate() override
        {
        }

        virtual void OnStop() override
        {
            DE_INFO("OSRobotsGame stopping");
        }
        
    private:
        GameGuiLayer *m_GameGuiLayer;
        EAGameGuiLayer *m_EAGameGuiLayer;
    };

} // namespace Application

DampEngine::Application *CreateApplication()
{
    return new Application::MyApplication();
}
