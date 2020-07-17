/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: Input System for the game.
Date: Feb/14/2020
*/

#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

#include "GameModel.h"

namespace ley {

class Input {

private:
  
public:
    Input();
    ~Input();

    ley::Direction pollEvents(bool&, bool&, GameModel&);
    ley::Direction pollEndEvents(bool&, bool&, GameModel&);
};

}
#endif
