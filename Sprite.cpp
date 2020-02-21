/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/14/2020
*/
#include "Sprite.h"

/* RAII */
ley::Sprite::Sprite(SDL_Renderer* r, const char* p, unsigned int s, std::vector<SDL_Rect>* v)
: Texture(r,p,s,v) {

}

ley::Sprite::~Sprite() {

}

/* Accessors */


/* Functions */
