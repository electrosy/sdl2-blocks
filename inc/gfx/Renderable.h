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

#include "../Screen.h"

namespace ley {

const SDL_Color CWHITE = {255, 255, 255, 255};
const SDL_Color CBRIGHTRED = {238, 51, 84, 255};
const SDL_Color CDARKTEAL = {32, 85, 83, 255};

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
