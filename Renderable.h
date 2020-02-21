/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

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

enum class Color {red,blue,green,orange,yellow,violet,indigo,white};

class Renderable {

private:
    
protected:
   SDL_Renderer* renderer;
  
public:
    /* RAII */
    Renderable(SDL_Renderer*);
    ~Renderable();
    /* Accessors */
    

    /* Functions */
    void virtual render() = 0;
};

}
#endif
