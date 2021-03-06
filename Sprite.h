/*
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: Representation of a sprite object on the screen. 
         This is the most basic game object that is 
         displayed on the screen
Date: Feb/17/2020
*/
#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Texture.h"

namespace ley {

class Sprite : public Texture {

private:
    
public:
    /* RAII */
    Sprite(SDL_Renderer* r, const char*, unsigned int = 0, std::vector<SDL_Rect>* = nullptr);
    ~Sprite();

    /* Accessors */

    /* Functions */
    void clear();
};

}
#endif
