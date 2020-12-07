#include "WText.hpp"

WText::WText(SDL_Renderer *renderer, const std::string& text): Widget(renderer) {
    this->font = OSDL::useFont(OSDL::FontPaths::OPEN_SANS, 500); //"font definition (ptsize)"
    setText(text);
}

WText::~WText() { 
    SDL_DestroyTexture(texture);
}

void WText::setText(const std::string& text) {
    TTF_Font *ttf_font = font->getSDLFont();

    //TODO: look up how to break line
    SDL_Surface *surface = TTF_RenderUTF8_Blended(ttf_font, text.c_str(), {0x00,0x00,0x00,0xFF});
    if (surface == NULL) {
        throw std::runtime_error("error: surface");
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        throw std::runtime_error("error: texture");
    }

    SDL_FreeSurface(surface);

    this->requestUpdate();  
}

// == Virtual overrides ==

void WText::mount() {

}

void WText::render() const {
    SDL_RenderCopy(renderer, texture, NULL, &transform);
}

void WText::update() {
    SDL_QueryTexture(texture, NULL, NULL, &transform.w, &transform.h);

    //TODO: find wich unit it is using (px?)
    const int lineHeight = 50; 

    float whRatio = (float)transform.w/transform.h;

    transform.w = whRatio * lineHeight;
    transform.h = lineHeight;
}