/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/14/2020
*/
#include "Sprite.h";

/* RAII */
ley::Sprite::Sprite(SDL_Renderer* r, const char* path, bool m, std::vector<SDL_Rect>* v) {
    renderer=r;
    multiframe=m;

    SDL_Surface* temp_surface = IMG_Load(path);
    texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
    SDL_FreeSurface(temp_surface);
    

    SDL_Rect source_rect;    
    dest_rect.y = source_rect.y = 0;

    if(multiframe) {


        for(auto rect : *v) {
            source_rect.x = rect.x; source_rect.y = rect.y;
            source_rect.w = dest_rect.w = rect.w;
            source_rect.h = dest_rect.h = rect.h;

            frames.push_back(source_rect);
        }
       // source_rect.x = (*v).at(0).x; source_rect.y = (*v).at(0).y;
       // source_rect.w = dest_rect.w = (*v).at(0).w;
       // source_rect.h = dest_rect.h = (*v).at(0).h;
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
void ley::Sprite::render(int x, int y, int frame_num) {
    dest_rect.x = x;
    dest_rect.y = y;
    SDL_RenderCopy(renderer, texture, &frames[frame_num], &dest_rect);
}