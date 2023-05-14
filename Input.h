/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: Input System for the game.
Date: Feb/14/2020
*/

#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

namespace ley {

enum class Command {down,right,up,left,none,pause,space,console,cclockwise,clockwise,debugcolide,debugtexture,enter,quit}; //these include directions and other inputs, this is overloaded maybe a couple enums would work better?

class Input {

private:
  
public:
    Input();
    ~Input();
    // TODO we may not need to have all these different function,
    // there is likely a way to generalize them.
    ley::Command pollEvents(bool&, bool& playnext);
    ley::Command pollTitleEvents(bool&);
    ley::Command pollMainMenuEvents(bool&);
};

}
#endif
