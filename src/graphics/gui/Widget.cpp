#include "Widget.hpp"

#include <stdio.h>

Widget::Widget(SDL_Renderer *renderer, const SDL_Rect& transform) {
    this->renderer = renderer;
    this->eventHandler = new EventHandler();
    setTransform(transform);
}

Widget::~Widget() {
    for (auto item: innerWidgets) {
        delete item;
    }
    delete eventHandler;
}

EventHandler *Widget::getEventHandler() {
    return eventHandler;
}

#pragma region Trasform

void Widget::setTransform(const SDL_Rect& transform) {
    this->transform = transform;
}

SDL_Rect Widget::getTransform() const {
    return this->transform;
}

void Widget::setPosition(const Vec2D& position) {
    transform.x = position.x;
    transform.y = position.y;
}

Vec2D Widget::getPosition() const {
    return {transform.x, transform.y};
}

void Widget::setDimention(const Vec2D& dimension) {
    transform.w = dimension.x;
    transform.h = dimension.y;
}

Vec2D Widget::getDimension() const {
    return {transform.w, transform.h};
}

#pragma endregion Transform

#pragma region Virtual_Methods
void Widget::mount() { }
void Widget::render() const { }
void Widget::update() { }

#pragma endregion Virtual_Methods

#pragma region Child_Propagation

void Widget::requestMount() { 
    this->mount();

    for (auto item: innerWidgets) {
        item->requestMount();
    }
}

void Widget::requestRender(bool renderChildren) const {
    this->render();

    if (renderChildren) for (auto item: innerWidgets) {
        item->requestRender();
    }
}

void Widget::requestUpdate() { 
    this->update();

    for (auto item: innerWidgets) {
        item->requestUpdate();
    }
}

#pragma endregion Child_Propagation


