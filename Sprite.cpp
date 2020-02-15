/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/14/2020
*/

#include "Sprite.h";

/* RAII */
ley::Sprite::Sprite(SDL_Renderer* r, const char* path) {

    renderer=r;

    SDL_Surface* temp_surface = IMG_Load(path);
    texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
    SDL_FreeSurface(temp_surface);
    SDL_QueryTexture(texture, NULL, NULL, &source_rect.w, &source_rect.h);
    dest_rect.x = source_rect.x = 0;
    dest_rect.y = source_rect.y = 0;
    dest_rect.w = source_rect.w;
    dest_rect.h = source_rect.h;
}

ley::Sprite::~Sprite() {

}

/* Functions */
void ley::Sprite::render(int x, int y) {
    dest_rect.x = x;
    dest_rect.y = y;
    SDL_RenderCopy(renderer, texture, &source_rect, &dest_rect);
}