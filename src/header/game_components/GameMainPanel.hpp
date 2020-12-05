#include "WPanel.hpp"
#include "Game.fwd.hpp"
#include <iostream>

class GameMainPanel : public WPanel {
    private:
        const Game& game;
    
    public:
        GameMainPanel(const Game& game);
        void mount();
};