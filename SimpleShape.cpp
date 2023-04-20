/* 
sdl2-blocks
Copyright (C) 2020,2023 Steven Philley

Purpose: see header.
Date: Mar/27/2023
*/
#include "SimpleShape.h"

/* RAII */
ley::SimpleShape::SimpleShape()
: Renderable() {
    
    rect_tex.w = -1;
    rect_tex.h = -1;
    destRect_tex.w = -1;
    destRect_tex.h = -1;
}

ley::SimpleShape::~SimpleShape() {

}

void ley::SimpleShape::operator()(SDL_Renderer* r) {
    
}

/* Accessors */
void ley::SimpleShape::addShape(std::string id, SDL_Rect rect) {
  shapes.insert(std::make_pair(id,rect));
}

/* Functions */

/* private */

/* public */
void ley::SimpleShape::render(SDL_Renderer * r) {

    SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE);

    for (auto& [key, value]: shapes) {
        SDL_RenderDrawRect(r, &value);
    }

    //also render out our texture piece.
    if(rect_tex.h != -1 && rect_tex.w != -1 && destRect_tex.h != -1 && destRect_tex.w != -1) {
        SDL_RenderCopy(r, tex, &rect_tex, &destRect_tex);
    }
}