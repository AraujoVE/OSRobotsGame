#include "mypch.hpp"
#include <DampEngine/Core/EntryPoint.hpp>

#include "Application/ImGui/GameGuiLayer.hpp"
#include "Application/ImGui/EAGameGuiLayer.hpp"
#include "Application/Game/GameConsts.hpp"

#include "EvoAlg/ScriptConverter.hpp"

#include "Application/Game/Ingame/RobotsManagement.hpp"
#include "Application/Game/Ingame/VillageStats.hpp"
#include "Application/Game/GameConsts.hpp"
#include "Application/Game/Ingame/RobotFunctions.hpp"

#include "Application/Util/path.hpp"

#ifndef BOOST_VERSION
#error "Boost needs to be installed on the system (tested with version 1.75)"
#endif //!BOOST_VERSION

//TODO: stop using optional to avoid cpphack.hpp


//TODO: TICK_DELAY for Task and VilageStats:;decay
//TODO: OnGameEnded -> sends VilageStats::decay threadloop tickCount to callback as "gameDurationInTicks"

//TODO: unregister events

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


            // Simple Game Code:
            GameConsts *gameConsts = new GameConsts();
            gameConsts->LoadValuesFromFile(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_CONSTS));
            m_GameGuiLayer = new GameGuiLayer();
            GameRunner *runner = new GameRunner(gameConsts);
            m_GameGuiLayer->SetGameRunner(runner);
            runner->Start();
            m_LayerStack.PushOverlay(m_GameGuiLayer);
           
            // EA Code:
            // m_GameGuiLayer = new GameGuiLayer();
            // m_EAGameGuiLayer = new EAGameGuiLayer();
            // m_LayerStack.PushOverlay(m_EAGameGuiLayer);
            
            // auto *l_LayerStack = &m_LayerStack;
            // auto *l_GameGuiLayer = m_GameGuiLayer;

            // m_EAGameGuiLayer->SetOnSettingsChanged(new EH_EAGameSettingsChanged([=](EAGameSettings newSettings) {
            //     if (newSettings.ShowGame)
            //         l_LayerStack->PushOverlay(l_GameGuiLayer);
            //     else
            //         l_LayerStack->PopOverlay(l_GameGuiLayer);
            //     return false;
            // }));           
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
