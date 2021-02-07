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


//TODO: remove test includes
#include "EvoAlg/Threads/ThreadController.hpp"
#include "EvoAlg/ScriptRunner.hpp"
#include "EvoAlg/ScriptConverter.hpp"
#include "EvoAlg/Script.hpp"
#include "EvoAlg/Types.hpp"


//TODO: stop using optional to avoid cpphack.hpp

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

        //TODO: Jogo versão normal ou calibração
        virtual void OnStart() override
        {
            DE_TRACE("MyApplication::OnStart()");
        }

        virtual void InitLayers() override
        {
            DE_TRACE("MyApplication::InitLayers()");

            // Simple Game Code:
            // GameConsts *gameConsts = new GameConsts();
            // gameConsts->LoadFromFile(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_CONSTS));
            // gameConsts->SetTickDelay(500e3);
            // m_GameGuiLayer = new GameGuiLayer();
            // GameRunner *runner = new GameRunner(gameConsts);
            // m_GameGuiLayer->SetGameRunner(runner);
            // runner->Start();
            // m_LayerStack.PushOverlay(m_GameGuiLayer);

            // EA Code:
            // m_GameGuiLayer = new GameGuiLayer();
            // m_EAGameGuiLayer = new EAGameGuiLayer();
            // m_LayerStack.PushOverlay(m_EAGameGuiLayer);

            // auto *l_LayerStack = &m_LayerStack;
            // auto *l_GameGuiLayer = m_GameGuiLayer;

            // m_EAGameGuiLayer->SetOnSettingsChanged(new EH_EAGuiPropsChanged([=](const EvoAlg::EAGuiProps& newSettings) {
            //     if (newSettings.ShowGame)
            //         l_LayerStack->PushOverlay(l_GameGuiLayer);
            //     else
            //         l_LayerStack->PopOverlay(l_GameGuiLayer);

            //     l_GameGuiLayer->SetGameRunner(newSettings.MainGameRunner);
            //     return false;

            // }));
        }

        virtual void OnUpdate() override
        {
            Stop();
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
    GameConsts *gameConsts = new GameConsts();
    gameConsts->LoadFromFile(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_CONSTS));
    
    GameRunner *gameRunner = new GameRunner(gameConsts);

    EvoAlg::ScriptConverter scriptConverter(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_SCRIPT_HUMAN_FOLDER));
    EvoAlg::Script *script = scriptConverter.Convert();
    EvoAlg::ScriptRunner scriptRunner(*script);
    

    EvoAlg::GeneVec genes = gameConsts->SaveToCromossome();
    EvoAlg::Individual indv{0, genes};    

    int test = 1;

    if (test == 1) {
        gameRunner->Start();

        // usleep(10e6);
    }


    // scriptRunner.RunAllGameplays(*gameRunner, indv);

    DE_TRACE("Deleting script");
    delete script;

    DE_TRACE("Deleting gameRunner");
    delete gameRunner;
    DE_TRACE("Deleting gameConsts");
    delete gameConsts;








    DE_TRACE("Ending script");
    exit(0);
    return nullptr;
    return new Application::MyApplication();
}
