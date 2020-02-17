/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/17/2020
*/
#include "Texture.h";

/* RAII */
ley::Texture::Texture(SDL_Renderer* r,  const char* p, std::vector<SDL_Rect>* v) : Renderable(r)  {
    SDL_Surface* temp_surface = IMG_Load(p);
    texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
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