#include "GameMainPanel.hpp"
#include "VillageStatusPanel.hpp"
#include "Game.hpp"


//TODO: panel size according to game window
GameMainPanel::GameMainPanel(const Game& game) : WPanel(0,0,1000,1000), game(game) {}

void GameMainPanel::mount() {
    this->innerPanels.push_back(new VillageStatusPanel(game.getVillageStatus()));
}
