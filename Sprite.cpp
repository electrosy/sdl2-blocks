/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/14/2020
*/
#include "Sprite.h"
ley::Sprite::Sprite()
:
Renderable() {
    SDL_Log("Sprite() called");
}

ley::Sprite::Sprite(SDL_Texture * t, unsigned int s, std::vector<SDL_Rect> v)
: 
Renderable(),
animSpeed(s),
texture(t) {

    SDL_Log("Sprite(SDL_Texture * t, unsigned int s, std::vector<SDL_Rect> v) called");

    if (!texture) {
        return;//EARLY EXIT
    }

    bool multiframe = false;
    
    multiframe = v.size() > 1;
    
    SDL_Rect source_rect;
    dest_rect.y = source_rect.y = 0;
    if(multiframe) {
        for(auto rect : v) {
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

ley::Sprite::~Sprite() {

}

/* Accessors */
void ley::Sprite::setPos(unsigned int x, unsigned int y) {
    pos.first = x; pos.second = y;
}

void ley::Sprite::render(SDL_Renderer * r) {
    unsigned int frameIndex = frames.size() > 1 ? 
        (SDL_GetTicks() / animSpeed) % frames.size() : 0;

    dest_rect.x = pos.first; dest_rect.y = pos.second;
    SDL_RenderCopy(r, texture, &frames[frameIndex], &dest_rect);
}

/* Functions */
