#include <DampEngine.hpp>

class MyApplication : public DampEngine::Application
{
public:
    MyApplication(): DampEngine::Application({"OSRobotGame"}) {
    }
    
    void InitLayers() override {
        m_LayerStack.PushOverlay(new DampEngine::ImGuiLayer());
    }

};

DampEngine::Application *CreateApplication() {
    return new MyApplication();
}
