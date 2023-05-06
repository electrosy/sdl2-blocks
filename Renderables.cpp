/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/17/2020
*/
#include "Renderables.h"

/* RAII */
ley::Renderables::Renderables() {
    
}
 
ley::Renderables::~Renderables() {

}
/* Accessors */
unsigned int ley::Renderables::size() {
   
   return renderables.size() > 0 ? renderables.size() : 0;
}

/* Functions */
void ley::Renderables::push_back(Renderable * r) {
    
    renderables.push_back(r);
}

void ley::Renderables::renderAll(SDL_Renderer * r) {           
    
    if(renderables.size() > 0) {
        for(const auto value : renderables) {
            value->render(r);
        }
    }
}