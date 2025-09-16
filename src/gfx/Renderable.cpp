/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/17/2020
*/
#include "../../inc/gfx/Renderable.h"

/* RAII */
ley::Renderable::Renderable() {
   mVisible=true;
}

ley::Renderable::~Renderable() {

}
/* Accessors */
void ley::Renderable::setVisible(bool v) {
   mVisible = v;
}

bool ley::Renderable::isVisible() {
   return mVisible;
}

/* Functions */