/* 
Author: Steven Philley
Purpose: An object that can be rendered using SDL_RenderCopy
Date: Feb/17/2020
*/
#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace ley {

class Renderable {

private:
   
  
public:
    Renderable();
    ~Renderable();

    void virtual render() = 0;
};

}
#endif
