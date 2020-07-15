/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/20/2020
*/
#include "SimpleShape.h"

/* RAII */
ley::SimpleShape::SimpleShape(SDL_Renderer* r)
: Renderable(r) {
    
    rect_tex.w = -1;
    rect_tex.h = -1; 
    destRect_tex.w = -1;
    destRect_tex.h = -1;
}

ley::SimpleShape::~SimpleShape() {

}
/* Accessors */
void ley::SimpleShape::addShape(std::string id, SDL_Rect rect) {
  shapes.insert(std::make_pair(id,rect));
}

/* Functions */
/* private */
void ley::SimpleShape::drawRect(const SDL_Rect* r) {
    SDL_RenderDrawRect(renderer,r);
}
/* public */
void ley::SimpleShape::render() {

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    for (auto& [key, value]: shapes) {
        SDL_RenderDrawRect(renderer, &value);
    }

    //also render out our texture piece.
    if(rect_tex.h != -1 && rect_tex.w != -1 && destRect_tex.h != -1 && destRect_tex.w != -1) {
        SDL_RenderCopy(renderer, tex, &rect_tex, &destRect_tex);
    }
}