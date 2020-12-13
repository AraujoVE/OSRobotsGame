#include <DampEngine.hpp>

#include "ImGui/MainGuiLayer.hpp"

class MyApplication : public DampEngine::Application
{
public:
    MyApplication(): DampEngine::Application({"OSRobotGame"}) {
    }

    virtual void InitLayers() override {
        m_LayerStack.PushOverlay(new MainGuiLayer());
    }

    virtual void OnUpdate() override {

    }

};

DampEngine::Application *CreateApplication() {
    return new MyApplication();
}
