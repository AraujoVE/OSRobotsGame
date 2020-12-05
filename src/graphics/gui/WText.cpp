#include "WText.hpp"

WText::WText(SDL_Renderer *renderer, const std::string& text): Widget(renderer) {
    setText(text);
}

WText::~WText() { 
    SDL_DestroyTexture(texture);
}

void WText::setText(const std::string& text) {
    TTF_Font *font = FONT_MACRO;
    if (font == NULL) {
        throw std::runtime_error("error: font not found");
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), {0x00,0x00,0x00,0xFF});
    if (surface == NULL) {
        throw std::runtime_error("error: surface");
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        throw std::runtime_error("error: texture");
    }

    //TODO: make this unchangeable from outside if I have patience
    transform.w = surface->w/2;
    transform.h = surface->h/2;

    SDL_FreeSurface(surface);
    
}

// == Virtual overrides ==

void WText::mount() {

}

void WText::render() const {
    SDL_RenderCopy(renderer, texture, NULL, &transform);
}