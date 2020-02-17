/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/14/2020
*/
#include "Sprite.h";

/* RAII */
ley::Sprite::Sprite(SDL_Renderer* r, const char* p, std::vector<SDL_Rect>* v)
: Texture(r,p,v) {

}

ley::Sprite::~Sprite() {

}

/* Functions */