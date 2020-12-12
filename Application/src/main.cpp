#include <DampEngine.hpp>

#include <glm/glm.hpp>

class MyApplication : public DampEngine::Application
{
public:
    MyApplication(): DampEngine::Application({"OSRobotGame"}) {
    }

    virtual void InitLayers() override {
        m_LayerStack.PushOverlay(new DampEngine::ImGuiLayer());
    }

    virtual void OnUpdate() override {

    }

};

DampEngine::Application *CreateApplication() {
    return new MyApplication();
}
