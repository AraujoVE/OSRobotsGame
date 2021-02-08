#include "mypch.hpp"
#include <DampEngine/Core/EntryPoint.hpp>

#include "Application/ImGui/GameGuiLayer.hpp"
#include "Application/ImGui/EAGameGuiLayer.hpp"
#include "Application/Game/GameConsts.hpp"

#include "EvoAlg/Script/ScriptConverter.hpp"

#include "Application/Game/Ingame/RobotsManagement.hpp"
#include "Application/Game/Ingame/VillageStats.hpp"
#include "Application/Game/GameConsts.hpp"
#include "Application/Game/Ingame/RobotFunctions.hpp"

#include "Application/Util/path.hpp"

#ifndef BOOST_VERSION
#error "Boost needs to be installed on the system (tested with version 1.75)"
#endif //!BOOST_VERSION

//TODO: remove test includes
#include "Application/Threads/ThreadLoop.hpp"
#include "Application/Events/EventHandler/DefaultHandlers.hpp"
#include "EvoAlg/Threads/ThreadController.hpp"
#include "EvoAlg/Script/ScriptRunner.hpp"
#include "EvoAlg/Script/ScriptConverter.hpp"
#include "EvoAlg/Script/Script.hpp"
#include "EvoAlg/Types.hpp"

//TODO: stop using optional to avoid cpphack.hpp

//TODO: unregister events. done?

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
            m_GameGuiLayer = new GameGuiLayer();
            m_EAGameGuiLayer = new EAGameGuiLayer();
            m_LayerStack.PushOverlay(m_EAGameGuiLayer);

            auto *l_LayerStack = &m_LayerStack;
            auto *l_GameGuiLayer = m_GameGuiLayer;

            m_EAGameGuiLayer->SetOnSettingsChanged(new EH_EAGuiPropsChanged([=](const EvoAlg::EAGuiProps& newSettings) {
                if (newSettings.ShowGame)
                    l_LayerStack->PushOverlay(l_GameGuiLayer);
                else
                    l_LayerStack->PopOverlay(l_GameGuiLayer);

                l_GameGuiLayer->SetGameRunner(newSettings.MainGameRunner);
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

    // uint64_t tests = 1;

    // while (tests-- > 0)
    // {
    //     int test = 1;

    //     if (test == 0) {
    //         GameConsts *gameConsts = new GameConsts();
    //         gameConsts->LoadFromFile(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_CONSTS));
    //         gameConsts->SetTickDelay(1);
    //         GameRunner *gameRunner = new GameRunner(gameConsts);

    //         DE_INFO("[Main] Starting game...");
    //         gameRunner->Start();
    //         DE_INFO("[Main] Stopping game...");
    //         gameRunner->Stop();
    //         DE_INFO("[Main] Game Stopped successfully!");

    //         DE_TRACE("Deleting gameRunner");
    //         delete gameRunner;
    //         DE_TRACE("Deleting gameConsts");
    //         delete gameConsts;

    //     }
    //     else if (test == 1)
    //     {
    //         GameConsts *gameConsts = new GameConsts();
    //         gameConsts->LoadFromFile(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_CONSTS));
    //         gameConsts->SetTickDelay(1);

    //         GameRunner *gameRunner = new GameRunner(gameConsts);

    //         EvoAlg::ScriptConverter scriptConverter(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_SCRIPT_HUMAN_FOLDER));
    //         EvoAlg::Script *script = scriptConverter.Convert();
    //         EvoAlg::ScriptRunner scriptRunner(*script);

    //         EvoAlg::GeneVec genes = gameConsts->SaveToCromossome();
    //         EvoAlg::Individual indv{0, genes};

    //         scriptRunner.RunAllGameplays(*gameRunner, indv);

    //         DE_TRACE("Deleting script");
    //         delete script;

    //         DE_TRACE("Deleting gameRunner");
    //         delete gameRunner;
    //         DE_TRACE("Deleting gameConsts");
    //         delete gameConsts;


    //     }
    //     else if (test == 2) {
    //         DE_INFO("Start test 2");
    //         ThreadLoopParams *tlp = new ThreadLoopParams([]{}, []{return true;}, 1); 
    //         ThreadLoop *threadLoop = new ThreadLoop("Testing");
            
    //         EH_ThreadEnded *teeh = new EH_ThreadEnded([=](ThreadEndedReason::ThreadEndedReason_t reason){
    //             DE_TRACE("ThreadLoop end callback. reason: {0}", reason);
    //             return false;
    //         });

    //         threadLoop->m_EventListener->Register(teeh);
    //         threadLoop->Start(tlp);
    //         threadLoop->Stop();
    //         delete threadLoop;
    //         DE_INFO("End test 2");

    //     }
    //     else if (test == 3) {
    //         DE_INFO("Start test 3");
    //         ThreadLoopParams *tlp = new ThreadLoopParams([]{}, []{return false;}, 500e3); 
    //         ThreadLoop *threadLoop = new ThreadLoop("Testing");
            
    //         threadLoop->Start(tlp);
    //         usleep(3e2);
    //         threadLoop->Stop();
    //         // threadLoop->Start(tlp);

    //         delete threadLoop;
    //         DE_INFO("End test 3");

    //     }





    //     else if (test == -1) {
    //         DE_INFO("Start test 4");
    //         ThreadLoopParams *tlp = new ThreadLoopParams([]{}, []{return false;}, 500e3); 
    //         ThreadLoop *threadLoop = new ThreadLoop("Testing");
            
    //         threadLoop->Start(tlp);
    //         usleep(3e2);
    //         threadLoop->Abandon(); //This should not work, if yet disabled
    //         threadLoop->Start(tlp);

    //         delete threadLoop;
    //         DE_INFO("End test 4");

    //     }

    // }

    // DE_TRACE("Ending Application");
    // exit(0);
    // return nullptr;
    return new Application::MyApplication();
}
