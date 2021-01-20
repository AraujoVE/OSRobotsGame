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
        GameRunner *m_GameRunner;

    public:
        MyApplication() : DampEngine::Application({"OSRobotGame"})
        {
            DE_INFO("Initializing Application...");
            m_GameRunner = new GameRunner();
        }

        ~MyApplication()
        {
            delete m_GameRunner;
        }

        virtual void OnStart() override
        {
            DE_INFO("MyApplication::OnStart()");
        }

        virtual void InitLayers() override
        {
            DE_INFO("MyApplication::InitLayers()");

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

///////////////// REMOVE DEBUG

            // GameRunner gr1, gr2;
            // gr1.Start();
            // gr2.Start();


            // GameConsts *gc = new GameConsts();
            // gc->LoadValuesFromFile("Application/config/constValues.cfg");

            // RobotsManagement *rm1 = new RobotsManagement(*gc);
            // RobotsManagement *rm2 = new RobotsManagement(*gc);

            // VillageStats *vs1 = new VillageStats(*gc);
            // VillageStats *vs2 = new VillageStats(*gc);
        
            // vs1->startStatsDecayment();
            // vs2->startStatsDecayment();

            // while (true) {
            //     DE_DEBUG("vs1 pop: {0}", vs1->getPopulation());
            //     DE_DEBUG("vs2 pop: {0}\n", vs2->getPopulation());
            //     usleep(1000);
            // }

            // rm1->setVillageStats(vs1);
            // rm2->setVillageStats(vs2);

            // rm1->createTask(RobotFunction::PROTECTION);
            // rm2->createTask(RobotFunction::PROTECTION);

            
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
