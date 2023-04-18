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
#include "Renderable.h"

namespace ley {

class Sprite : public Renderable {

private:
    std::pair<unsigned int,unsigned int> pos;
    unsigned int animSpeed;
protected:
    SDL_Texture* texture;
    std::vector<SDL_Rect> frames;
    SDL_Rect dest_rect;
public:
    Sprite();
    Sprite(SDL_Texture * t, unsigned int, std::vector<SDL_Rect>);
    ~Sprite();
    void render(SDL_Renderer * r);
    void setPos(unsigned int, unsigned int);
    unsigned int getX() {return pos.first;}; unsigned int getY() {return pos.second;};
};

}
#endif