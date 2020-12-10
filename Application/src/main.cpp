#include <DampEngine.hpp>

class MyApplication : public DampEngine::Application
{
public:
    MyApplication(): DampEngine::Application({"OSRobotGame"}) {
    }
    
    virtual void InitLayers() override {
        m_LayerStack.PushOverlay(new DampEngine::ImGuiLayer());
    }

    virtual void OnUpdate() override {

        auto[x, y] = DampEngine::Input::GetMousePos();

        DE_INFO("MousePos: {0},{0} :)", x, y);
    }

};

DampEngine::Application *CreateApplication() {
    return new MyApplication();
}
