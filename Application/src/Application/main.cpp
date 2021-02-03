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

//TODO: remove test includes
#include "EvoAlg/Threads/ThreadController.hpp"
#include "EvoAlg/ScriptRunner.hpp"
#include "EvoAlg/ScriptConverter.hpp"
#include "EvoAlg/Script.hpp"
#include "EvoAlg/Types.hpp"


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

///


            uint32_t a = 1e8;
            // Testing GameRunner, GamnSavew and GameConsts. result: has leak, but it's not that big
            while (a-- > 0)
            {
                GameConsts *gc = new GameConsts();
                GameRunner *gr = new GameRunner(gc);

                gr->Start();
                // usleep(50);
                // usleep(50);

                delete gr;
                delete gc;
                usleep(50);
            }

///
            Stop();
            // using namespace EvoAlg;

            // //TODO: these lines insede while to check leaks V
            // ScriptConverter sc(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_SCRIPT_HUMAN_FOLDER));
            // Script *script = sc.Convert();

            // ScriptRunner sr(*script);
            // ThreadController tc;
            // //TODO end: ^

            // GameConsts gc;
            // gc.LoadFromFile(Util::Path::getDefaultPath(Util::Path::ResourceType::GAME_CONSTS));
            // std::vector<double> genes = gc.SaveToCromossome();
            
            //TEST ThreadController
            // while (true) {
            //     for (unsigned int i = 0; i < 1; i++)
            //     {
            //         tc.AddIndividual({i, genes});
            //     }
            //     tc.ExecuteAllIndividuals(sr);
            //     usleep(50e3);                
            // }
            

///


            // gc.SetTickDelay(5e2);
            // GameRunner gr(&gc);

            // while (true) {
            //     for (unsigned int i = 0; i < 50; i++)
            //     {
            //         Individual idv{i, genes};
            //         std::vector<TimeResult> *indvResult = sr.RunAllGameplays(gr, idv); 
            //         delete indvResult;
            //     }
            // }
            


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
