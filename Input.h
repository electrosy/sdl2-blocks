/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: Input System for the game.
Date: Feb/14/2020
*/

#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <functional>
#include "./inc/TextEntry.h"
#include "./inc/Command.h"

namespace ley {

class Input {

private:
    bool anyInputsMatch(const Uint8* state, std::vector<Uint8>* inputs);
    SDL_GameController *mControllerPtr = nullptr;
  
public:
    Input();
    ~Input();
    // TODO we may not need to have all these different function,
    // there is likely a way to generalize them.
    ley::Command pollEvents(bool& fullscreen, ley::KeyBindings* bindings, ley::TextEntry* te, const std::function<void(ley::Command c)>& function); 
};

}
#endif
