#include "WImage.hpp"

#include <stdexcept>

WImage::WImage(SDL_Renderer *renderer, const std::string& imagePath): Widget(renderer) {
    setImage(imagePath);
}

WImage::~WImage() {
    SDL_DestroyTexture(texture);
}

void WImage::setImage(const std::string& imagePath) {
    this->imagePath = imagePath;

    SDL_Surface *surface = IMG_Load(imagePath.c_str());
    if (surface == nullptr) throw std::runtime_error("Unable to load file: " + imagePath + "\n\t" + IMG_GetError());

    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (this->texture == nullptr) throw std::runtime_error("Unable to create texture from surface (" + imagePath + ")\n\t" + IMG_GetError());
    
    this->transform.w = surface->w;
    this->transform.h = surface->h;

    SDL_FreeSurface(surface);
}

void WImage::render() const {
    SDL_RenderCopy(renderer, texture, NULL, &transform);
}