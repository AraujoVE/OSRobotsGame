#include "EAGuiProps.hpp"

#include "Application/ImGui/GameGuiLayer.hpp"

namespace EvoAlg
{
    void EAGuiProps::FlushSettings(EAController &controller)
    {

        {
            //TODO: refactor
            if (m_GameRunnerThreadMap.find(m_Settings.PreviewThreadID) != m_GameRunnerThreadMap.end())
            {
                Application::GameRunner *gameRunner = m_GameRunnerThreadMap.at(m_Settings.PreviewThreadID);
                if (gameRunner->IsGamePaused() ^ m_Settings.Paused)
                    gameRunner->Pause(m_Settings.Paused);

                Application::GameConsts &gameConsts = gameRunner->GetGameConsts();
                if (gameConsts.GetTickDelay() != m_Settings.SelectedTickDelay)
                    gameConsts.SetTickDelay(m_Settings.SelectedTickDelay);
            }
            // controller.Pause(); //pause algorithm?
        }
    }
} // namespace EvoAlg