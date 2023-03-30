/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/17/2020
*/
#include "Renderable.h"

/* RAII */
ley::Renderable::Renderable(SDL_Renderer* r) {
   renderer=r, visible=true;
}

ley::Renderable::Renderable() {
   
}

ley::Renderable::~Renderable() {

}
/* Accessors */
void ley::Renderable::setVisible(bool v) {
   visible = v;
}

bool ley::Renderable::isVisible() {
   return visible;
}

/* Functions */