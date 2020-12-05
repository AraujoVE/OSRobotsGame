#include "WTaskController.hpp"
#include "WText.hpp"

WTaskController::WTaskController(SDL_Renderer *renderer): Widget(renderer) {

}

void WTaskController::mount() {
    setOnClick([this](int x, int y) {
        
        int middle = transform.x + transform.w/2;

        if (x > middle)
            transform.x += 10;
        else 
            transform.x -= 10;


        return true;
    });

    WText *wText = new WText(renderer, "São 4 da manhã");

    innerWidgets.push_back(wText);
}

void WTaskController::render() const {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x99, 0x99, 0xFF);
    SDL_RenderFillRect(renderer, &transform); 
}
