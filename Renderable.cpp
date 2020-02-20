/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/17/2020
*/
#include "Renderable.h"

/* RAII */
ley::Renderable::Renderable(SDL_Renderer* r) {
   renderer=r;
}

ley::Renderable::~Renderable() {

}
/* Accessors */

/* Functions */