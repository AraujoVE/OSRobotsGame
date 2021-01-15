#include "mypch.hpp"
#include <DampEngine/Core/EntryPoint.hpp>

#include "Application/ImGui/GameGuiLayer.hpp"
#include "Application/ImGui/EAGameGuiLayer.hpp"
#include "Application/header/ConstsMap.hpp"

#include "EAAlgorithm/ConvertScripts.hpp"

#ifndef BOOST_VERSION
    #error "Boost needs to be installed on the system (tested with version 1.75)"
#endif  //!BOOST_VERSION


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
            getcwd(path, 1024);
            std::string cwd(path);
            ConstsMap::initMapFromFile(cwd + "/Application/config/constValues.cfg");

            EAAlgorithm::ConvertScripts convertScripts(cwd);
        }

        virtual void InitLayers() override
        {
            m_GameGuiLayer = new GameGuiLayer();
            m_LayerStack.PushOverlay(m_GameGuiLayer);
            // m_EAGameGuiLayer = new EAGameGuiLayer();
            // m_LayerStack.PushOverlay(m_EAGameGuiLayer);

            // auto *layerStack = &m_LayerStack;
            // auto gameLayer = m_GameGuiLayer;
            // m_EAGameGuiLayer->SetOnSettingsChanged(new EH_EAGameSettingsChanged([layerStack, gameLayer](EAGameSettings &newSettings) {
            //     if (newSettings.ShowGame)
            //         layerStack->PushOverlay(gameLayer);
            //     else
            //         layerStack->PopOverlay(gameLayer);
            //     return false;
            // }));
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
