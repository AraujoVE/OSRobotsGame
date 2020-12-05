#include "Game.hpp"
#include "GameGraphics.hpp"

#include "WVillageStatusPanel.hpp"
#include "WGameMainPanel.hpp"
#include "WImage.hpp"
#include "WTaskController.hpp"

//TODO: panel size according to game window
WGameMainPanel::WGameMainPanel(SDL_Renderer *renderer, const Game& game) : Widget(renderer, {0,0,1000,1000}), game(game) {}

void WGameMainPanel::mount() {
    this->innerWidgets.push_back(new WVillageStatusPanel(renderer, game.getVillageStatus()));
    
    WImage *waifu = new WImage(renderer, "/home/marucs/Development/USP/BCC/2020-2/SO/OSRobotsGame/assets/hitagi.png");

    int ww, wh;
    SDL_GetWindowSize(game.getGraphics()->getWindow(), &ww, &wh);
    SDL_Rect wd = waifu->getTransform();

    int rf = 1;
    waifu->setTransform({(int)(ww-wd.w/rf)/2, (int)(wh-wd.h/rf)/2 + 20, (int) wd.w/rf, (int) wd.h/rf});

    this->innerWidgets.push_back(waifu);

    WTaskController *taskController = new WTaskController(renderer);
    taskController->setTransform({40,40,100,100});

    innerWidgets.push_back(taskController);

}
