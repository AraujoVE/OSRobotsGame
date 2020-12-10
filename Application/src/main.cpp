#include <DampEngine.hpp>

class MyApplication : public DampEngine::Application
{
public:
    MyApplication(): DampEngine::Application({"OSRobotGame"}) {
    }
    
    virtual void InitLayers() override {
        m_LayerStack.PushOverlay(new DampEngine::ImGuiLayer());
    }

};

DampEngine::Application *CreateApplication() {
    return new MyApplication();
}
