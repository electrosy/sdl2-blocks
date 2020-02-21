/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A window within an SDL window concept for SDL2
Date: Feb/16/2020
*/
#ifndef WINLET_H
#define WINLET_H

#include <SDL2/SDL.h>

namespace ley {

class Winlet {

private:
    SDL_Rect bounds;
    SDL_Color boundsColor;
public:    
    Winlet(SDL_Rect, SDL_Color);
    ~Winlet();

    void render();
};

}
#endif
