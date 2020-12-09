
//Include all the engine
#include <DampEngine.hpp>

//Create your own application
class MyCustomApplication : public DampEngine::Application {
    void CreateWindow() override {
        
    }

    void OnStart() override {
        //Load game files
    }

    void OnUpdate() override {
        //Update all game state according to your game
    }

    void OnStop() override {
        //Save game files
    }
};

DampEngine::Application *CreateApplication() {
    return new MyCustomApplication();
}