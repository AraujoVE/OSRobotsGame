#include "mypch.hpp"
#include <DampEngine/Core/EntryPoint.hpp>

#include "ImGui/MainGuiLayer.hpp"

namespace Application
{
    class MyApplication : public DampEngine::Application
    {
    private:
        MyApplication *s_Instance = nullptr;
        std::unique_ptr<EAScript> m_EAScript;
    public:
        MyApplication() : DampEngine::Application({"OSRobotGame"})
        {
            
        }

        virtual void OnStart() override
        {
            DE_INFO("OSRobotsGame starting");
            ConstsMap::initMapFromFile("constValues.cfg");
            m_GameSave.reset(new GameSave());
        }

        virtual void InitLayers() override
        {
            m_MainGuiLayer = new MainGuiLayer(*m_GameSave.get());
            m_LayerStack.PushOverlay(m_MainGuiLayer);
        }

        virtual void OnUpdate() override
        { 
            static int a = 0;
            if (a++ > 100) {
                a = 0;
                m_GameSave->getRobotsManagement()->createTask(RobotFunction::HUNT);
            }
        }

        virtual void OnStop() override
        {
            DE_INFO("OSRobotsGame stopping");
        }
        
    private:
        std::unique_ptr<GameSave> m_GameSave;
        MainGuiLayer *m_MainGuiLayer;
    };

} // namespace Application

DampEngine::Application *CreateApplication()
{
    return new Application::MyApplication();
}

// bool isGameOver(VillageStats * village,  RobotsManagement * robotsManag) {
//     if (village->getPopulation() == 0 || robotsManag->getTotRobots() == 0) // if population is 0 or all robots were destroyed, game is over
//         return true;
//     return false;
// } 