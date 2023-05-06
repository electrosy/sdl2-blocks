/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A collection of all the renderable objects for the program.
Date: Feb/17/2020
*/
#ifndef RENDERABLES_H
#define RENDERABLES_H

#include <string>
#include <vector>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Renderable.h"

namespace ley {

typedef std::vector<Renderable*> RenderableType;

class Renderables {

private:
    RenderableType renderables;
protected:
   
public:
    Renderables();
    ~Renderables();

    void push_back(Renderable * r);
    void renderAll(SDL_Renderer * r);

    unsigned int size();
};

}
#endif
