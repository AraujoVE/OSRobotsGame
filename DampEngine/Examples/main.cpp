
//Include all the engine
#include <DampEngine.hpp>

//Create your own application
class MyCustomApplication : public DampEngine::Application {
    virtual void CreateWindow() override {
        this->Application::CreateWindow(); // If base class' method is not called, no window will be created by default
        //(Optional) override window creation logic
    }

    virtual void OnStart() override {
        //(Optional) Custom logic before main loop (ie. Load game files)
    }

    virtual void InitLayers() override {
        //(NOT OPTIONAL) init your custom layers here
        // ie. m_LayerStack.PushLayer(new DampEngine::Layer()) // won't compile for interface Layer, you must make a subclass
    }

    virtual void OnUpdate() override {
        //(Optional) Update all game state according to your game (called every frame)
    }

    virtual void OnStop() override {
        //(Optional) Custom logic before closing application (ie. Save game files)
    }
};

DampEngine::Application *CreateApplication() {
    return new MyCustomApplication();
}