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
    // TODO we may not need to have all these different function,
    // there is likely a way to generalize them.
    ley::Command pollEvents(bool&, GameModel* gm, bool& playnext);
    ley::Command pollEndEvents(bool&, GameModel&);
    ley::Command pollTitleEvents(bool&, GameModel&);
    ley::Command pollMainMenuEvents(bool&, GameModel&);
};

}
#endif
