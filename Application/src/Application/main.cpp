#include "mypch.hpp"
#include <DampEngine/Core/EntryPoint.hpp>

#include "ImGui/MainGuiLayer.hpp"

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
            ConstsMap::initMapFromFile("constValues.cfg");
        }

        virtual void InitLayers() override
        {
            m_MainGuiLayer = new MainGuiLayer(m_GameSave);
            m_LayerStack.PushOverlay(m_MainGuiLayer);
        }

        virtual void OnUpdate() override
        { 
            
        }

        virtual void OnStop() override
        {
            DE_INFO("OSRobotsGame stopping");
        }
        
    private:
        GameSave m_GameSave;
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