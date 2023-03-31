/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley. All rights reserved.

Purpose: see header.
Date: Feb/17/2020
*/
#include "Texture.h"

/* RAII */
ley::Texture::Texture(SDL_Renderer* r,  const char* p, unsigned int s, std::vector<SDL_Rect>* v)
: Renderable(), animSpeed(s) {

    if (p == nullptr) {return;//EARLY EXIT
    }

    SDL_Surface* temp_surface = IMG_Load(p);

    texture = SDL_CreateTextureFromSurface(r, temp_surface);

    SDL_FreeSurface(temp_surface);

    bool multiframe = false;
    if(v != nullptr) {
        multiframe = v->size() > 1;
    }

    SDL_Rect source_rect;
    dest_rect.y = source_rect.y = 0;
    if(multiframe) {
        for(auto rect : *v) {
            source_rect.x = rect.x; source_rect.y = rect.y;
            source_rect.w = dest_rect.w = rect.w; source_rect.h = dest_rect.h = rect.h;
            frames.push_back(source_rect);
        }
    } else {
        SDL_QueryTexture(texture, NULL, NULL, &source_rect.w, &source_rect.h);
        dest_rect.w = source_rect.w;
        dest_rect.h = source_rect.h;
        dest_rect.x = source_rect.x = 0;
    }
}

ley::Texture::~Texture() {

}

/* Accessors */
void ley::Texture::setPos(unsigned int x, unsigned int y) {
    pos.first = x; pos.second = y;
}

void ley::Texture::render(SDL_Renderer * r) {
    unsigned int frameIndex = frames.size() > 1 ? 
        (SDL_GetTicks() / animSpeed) % frames.size() : 0;

    dest_rect.x = pos.first; dest_rect.y = pos.second;
    SDL_RenderCopy(r, texture, &frames[frameIndex], &dest_rect);
}

/* Functions */