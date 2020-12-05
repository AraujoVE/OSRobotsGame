#include "WText.hpp"

WText::WText(SDL_Renderer *renderer, const std::string& text): Widget(renderer) {
    this->font = OSDL::useFont(OSDL::FontPaths::OPEN_SANS, 200);
    setText(text);
}

WText::~WText() { 
    SDL_DestroyTexture(texture);
}

void WText::setText(const std::string& text) {
    TTF_Font *ttf_font = font->getSDLFont();

    SDL_Surface *surface = TTF_RenderUTF8_Solid(ttf_font, text.c_str(), {0x00,0x00,0x00,0xFF});
    if (surface == NULL) {
        throw std::runtime_error("error: surface");
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        throw std::runtime_error("error: texture");
    }

    //TODO: make this unchangeable from outside if I have patience
    SDL_QueryTexture(texture, NULL, NULL, &transform.w, &transform.h);

    SDL_FreeSurface(surface);
    
}

// == Virtual overrides ==

void WText::mount() {

}

void WText::render() const {
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}