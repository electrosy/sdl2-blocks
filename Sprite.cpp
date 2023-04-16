/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/14/2020
*/
#include "Sprite.h"

/* RAII */
ley::Sprite::Sprite()
: Texture(nullptr, 0, nullptr) {

}

ley::Sprite::Sprite(SDL_Texture * t, unsigned int s, std::vector<SDL_Rect>* v)
: Texture(t, s, v) {

}

ley::Sprite::~Sprite() {

}

/* Accessors */


/* Functions */
