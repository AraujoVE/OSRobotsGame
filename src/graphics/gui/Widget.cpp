#include "Widget.hpp"

#include <stdio.h>

Widget::Widget(SDL_Renderer *renderer): Widget(renderer, {0,0,0,0}) {}

Widget::Widget(SDL_Renderer *renderer, const SDL_Rect& transform) {
    this->clickCallback = nullptr;
    this->renderer = renderer;
    setTransform(transform);
}

Widget::~Widget() {
    for (auto item: innerWidgets) {
        delete item;
    }
}

void Widget::setTransform(const SDL_Rect& transform) {
    this->transform = transform;
}


SDL_Rect Widget::getTransform() const {
    return this->transform;
}

//Default implementation for virtual method
void Widget::render() const{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &transform);
    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
    SDL_RenderDrawRect(renderer, &transform);
}

//Default implementation for virtual method: do nothing
void Widget::mount() { }

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

void Widget::setOnClick(OnClickCallback callback) {
    this->clickCallback = callback;
}

void Widget::onClick(int x, int y) const{
    if (x < transform.x || x > transform.x + transform.w ) return;
    if (y < transform.y || y > transform.y + transform.h ) return;

    printf("Widget %p received click! at (%d, %d)\n", this, x, y);

    bool passToChildren = true;
    if (clickCallback != nullptr)
        passToChildren = clickCallback(x, y);

    if (passToChildren) {
        for (auto child: innerWidgets) {
            child->onClick(x, y);
        }
    }
}