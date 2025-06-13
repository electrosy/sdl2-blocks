/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/17/2020
*/
#include "Renderables.h"

/* RAII */
ley::RenderablesPtr::RenderablesPtr() {
    
}
 
ley::RenderablesPtr::~RenderablesPtr() {

}
/* Accessors */
unsigned int ley::RenderablesPtr::size() {
   
   return renderables.size() > 0 ? renderables.size() : 0;
}

/* Functions */
void ley::RenderablesPtr::push_back(Renderable * r) {
    
    renderables.push_back(r);
}

void ley::RenderablesPtr::renderAll(SDL_Renderer * r, bool d) {           
    
    if(renderables.size() > 0) {
        for(const auto value : renderables) {
            value->render(r, d);
        }
    }
}