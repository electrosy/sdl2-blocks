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
    bool visible;
    
protected:
  
public:
    /* RAII */
    Renderable();
    ~Renderable();
    /* Accessors */
    void setVisible(bool);
    bool isVisible();

    /* Functions */
    void virtual render(SDL_Renderer * r, bool d) = 0; // d = debug
};

}
#endif
