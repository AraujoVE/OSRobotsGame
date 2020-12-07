#include "EventHandler.hpp"

EventHandler::EventHandler() {
}

EventHandler::~EventHandler() {

}

void EventHandler::setOnMouseDownCallback(MouseClickEventCallback callback) {
    if (mouseDownCallback != nullptr) {
        //TODO:warning: changing callback
    }
    mouseDownCallback = callback;
}

void EventHandler::setOnMouseUpCallback(MouseClickEventCallback callback) {
    if (mouseUpCallback != nullptr) {
        //TODO:warning: changing callback
    }
    mouseUpCallback = callback;
}

void EventHandler::setOnMouseMovedCallback(MouseMovementEventCallback callback) {
    if (mouseMovedCallback != nullptr) {
        //TODO:warning: changing callback
    }
    mouseMovedCallback = callback;
}

void EventHandler::setOnMouseHoverInCallback(MouseMovementEventCallback callback) {
    if (mouseHoverInCallback != nullptr) {
        //TODO:warning: changing callback
    }
    mouseHoverInCallback = callback;
}

void EventHandler::setOnMouseHoverOutCallback(MouseMovementEventCallback callback) {
    if (mouseHoverOutCallback != nullptr) {
        //TODO:warning: changing callback
    }
    mouseHoverOutCallback = callback;
}
