#include "mypch.hpp"
#include <DampEngine/Core/EntryPoint.hpp>

#include "Application/ImGui/GameGuiLayer.hpp"
#include "Application/ImGui/EAGameGuiLayer.hpp"
#include "Application/header/ConstsMap.hpp"

#include "EAAlgorithm/ConvertScripts.hpp"

#include "Application/header/RobotsManagement.hpp"
#include "Application/header/VillageStats.hpp"
#include "Application/header/ConstsMap.hpp"
#include "Application/header/RobotFunctions.hpp"

#ifndef BOOST_VERSION
#error "Boost needs to be installed on the system (tested with version 1.75)"
#endif //!BOOST_VERSION



//TODO: TICK_DELAY for Task and VilageStats:;decay
//TODO: OnGameEnded -> sends VilageStats::decay threadloop tickCount to callback as "gameDurationInTicks"

namespace Application
{
    class MyApplication final : public DampEngine::Application
    {
    private:
        MyApplication *s_Instance = nullptr;

    public:
        MyApplication() : DampEngine::Application({"OSRobotGame"})
        {
            DE_INFO("Initializing Application...");
        }

        ~MyApplication()
        {
            DE_TRACE("MyApplication::~MyApplication()");
        }

        virtual void OnStart() override
        {
            DE_TRACE("MyApplication::OnStart()");
        }

        virtual void InitLayers() override
        {
            DE_TRACE("MyApplication::InitLayers()");

            m_GameGuiLayer = new GameGuiLayer();
            m_EAGameGuiLayer = new EAGameGuiLayer();

            // m_LayerStack.PushOverlay(m_GameGuiLayer);
            // m_GameRunner->Start();
            
            m_LayerStack.PushOverlay(m_EAGameGuiLayer);
            
            auto *l_LayerStack = &m_LayerStack;
            auto *l_GameGuiLayer = m_GameGuiLayer;

            m_EAGameGuiLayer->SetOnSettingsChanged(new EH_EAGameSettingsChanged([=](EAGameSettings newSettings) {
                if (newSettings.ShowGame)
                    l_LayerStack->PushOverlay(l_GameGuiLayer);
                else
                    l_LayerStack->PopOverlay(l_GameGuiLayer);

                //TODO: pause and unpause if game is attached (probably just pass to MainGameLayer as a Pause() method)
                // if (newSettings.PauseGame)
                //     l_GameRunner->Pause();
                // else
                //     l_GameRunner->Unpause();

                return false;
            }));           
        }

        virtual void OnUpdate() override
        {
        }

        virtual void OnStop() override
        {
            DE_INFO("MyApplication::OnStop()");

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
