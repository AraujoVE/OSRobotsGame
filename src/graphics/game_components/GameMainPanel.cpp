#include "GameMainPanel.hpp"
#include "Game.hpp"
#include "GameGraphics.hpp"

#include "VillageStatusPanel.hpp"
#include "WImage.hpp"

//TODO: panel size according to game window
GameMainPanel::GameMainPanel(SDL_Renderer *renderer, const Game& game) : WPanel(renderer, {0,0,1000,1000}), game(game) {}

void GameMainPanel::mount() {
    this->innerPanels.push_back(new VillageStatusPanel(renderer, game.getVillageStatus()));
    
    WImage *waifu = new WImage(renderer, "/home/marucs/Development/USP/BCC/2020-2/SO/OSRobotsGame/assets/hitagi.png");

    int ww, wh;
    SDL_GetWindowSize(game.getGraphics()->getWindow(), &ww, &wh);
    SDL_Rect wd = waifu->getTransform();
    waifu->setTransform({(ww-wd.w)/2, (wh-wd.h)/2 + 20, wd.w, wd.h});

    this->innerPanels.push_back(waifu);
}
