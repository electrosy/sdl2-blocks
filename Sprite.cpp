/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/14/2020
*/
#include "Sprite.h";

/* RAII */
ley::Sprite::Sprite(SDL_Renderer* r, const char* p, bool m, std::vector<SDL_Rect>* v)
: Texture(r,p) {
    multiframe=m;

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

ley::Sprite::~Sprite() {

}

/* Functions */