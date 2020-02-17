/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/17/2020
*/
#include "Texture.h";

/* RAII */
ley::Texture::Texture(SDL_Renderer* r,  const char* p) : Renderable(r)  {
    SDL_Surface* temp_surface = IMG_Load(p);
    texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
    SDL_FreeSurface(temp_surface);
}

ley::Texture::~Texture() {

}

/*
void ley::Texture::render() {

}
*/
void ley::Texture::render(int x, int y, int frame_num) {
    dest_rect.x = x;
    dest_rect.y = y;
    SDL_RenderCopy(renderer, texture, &frames[frame_num], &dest_rect);
}


/* Functions */