#include "mypch.hpp"
#include <DampEngine/Core/EntryPoint.hpp>

#include "Application/ImGui/GameGuiLayer.hpp"
#include "Application/ImGui/EAGameGuiLayer.hpp"
#include "Application/header/ConstsMap.hpp"

#include "EAAlgorithm/ConvertScripts.hpp"

#ifndef BOOST_VERSION
#error "Boost needs to be installed on the system (tested with version 1.75)"
#endif //!BOOST_VERSION

namespace Application
{
    class MyApplication final : public DampEngine::Application
    {
    private:
        MyApplication *s_Instance = nullptr;
        GameRunner *m_GameRunner;

    public:
        MyApplication() : DampEngine::Application({"OSRobotGame"})
        {
            m_GameRunner = new GameRunner();
        }

        ~MyApplication()
        {
            delete m_GameRunner;
        }

        virtual void OnStart() override
        {
            DE_INFO("OSRobotsGame starting");

        }

        virtual void InitLayers() override
        {
            m_GameGuiLayer = new GameGuiLayer(*m_GameRunner);
            m_EAGameGuiLayer = new EAGameGuiLayer(*m_GameRunner);

            // m_LayerStack.PushOverlay(m_GameGuiLayer);
            // m_GameRunner->Start();
            
            m_LayerStack.PushOverlay(m_EAGameGuiLayer);
            
            auto *l_LayerStack = &m_LayerStack;
            auto *l_GameGuiLayer = m_GameGuiLayer;
            auto *l_GameRunner = m_GameRunner;

            m_EAGameGuiLayer->SetOnSettingsChanged(new EH_EAGameSettingsChanged([=](EAGameSettings newSettings) {
                if (newSettings.ShowGame)
                    l_LayerStack->PushOverlay(l_GameGuiLayer);
                else
                    l_LayerStack->PopOverlay(l_GameGuiLayer);

                if (newSettings.PauseGame)
                    l_GameRunner->Pause();
                else
                    l_GameRunner->Unpause();

                return false;
            }));
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
