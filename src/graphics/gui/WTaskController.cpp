#include "WTaskController.hpp"

#include "Task.hpp"

#include <string>

WTaskController::WTaskController(SDL_Renderer *renderer): Widget(renderer) {
    this->wRobotCountText = nullptr;
    this->robotCount = 0;
}

void WTaskController::increaseRobots() { this->robotCount++; }
void WTaskController::decreaseRobots() { this->robotCount--; }

void WTaskController::mount() {
    // setOnClick([this](const Vec2D& point) {
    //     const static bool passEventToChildren = false;

    //     Vec2D relativeClickPos = point.relativeTo(this->getPosition());
    //     int relX = relativeClickPos.x, relY = relativeClickPos.y;

    //     if (relY < transform.h/2) {
    //         Vec2D delta = (relativeClickPos - getDimension()/2);
    //         this->setPosition(getPosition() + delta);
    //     } else {
    //         if (relX > transform.w/2) {
    //             this->increaseRobots();
    //         } else {
    //             this->decreaseRobots();
    //         }
    //     }

    //     this->requestUpdate();

    //     return passEventToChildren;
    // });

    this->wRobotCountText = new WText(renderer, std::to_string(robotCount));

    innerWidgets.push_back(wRobotCountText);
}

void WTaskController::render() const {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x99, 0x99, 0xFF);
    SDL_RenderFillRect(renderer, &transform); 
}


void WTaskController::update() {
    wRobotCountText->setText(std::to_string(robotCount));
    wRobotCountText->setPosition(getPosition());
}
