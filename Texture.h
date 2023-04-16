/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley. All rights reserved.

Purpose: Wrap SDL Texture which is a Renderable. Renderables
         are renderable with SDL_RenderCopy. A Texture is a Renderable
Date: Feb/17/2020
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Renderable.h"

namespace ley {

class Texture : public Renderable {

private:
    std::pair<unsigned int,unsigned int> pos;
    unsigned int animSpeed;
protected:
    SDL_Texture* texture;
    std::vector<SDL_Rect> frames;
    SDL_Rect dest_rect;
public:
    Texture(SDL_Texture * t, unsigned int, std::vector<SDL_Rect>*);
    ~Texture();
    void render(SDL_Renderer * r);
    void setPos(unsigned int, unsigned int);
    unsigned int getX() {return pos.first;}; unsigned int getY() {return pos.second;};
};

}
#endif
