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
#include "queue"
#include "map"

namespace ley {

class Input {

private:
    bool anyInputsMatch(const Uint8 scancode, std::vector<Uint8>* inputs);
    bool anyInputsMatch(const Uint8* state, std::vector<Uint8>* inputs);
    SDL_GameController *mControllerPtr = nullptr;
    std::map<Uint8, std::tuple<bool, ley::Timer, ley::Timer>> mKeysPressed; //keyboard
    std::map<Uint8, std::tuple<bool, ley::Timer, ley::Timer>> mButtonsPressed; //gamepad
    
public:
    Input();
    ~Input();
    ley::Command pollEvents(bool& fullscreen, ley::KeyBindings* bindings, std::queue<ley::Command>* commandQueuePtr, ley::TextEntry* te, const std::function<void(ley::Command c)>& function); 
    ley::Command pollEvents2(bool& fullscreen, ley::KeyBindings* bindings, ley::ButtonBindings* buttonBindings,  std::queue<ley::Command>* commandQueuePtr, ley::TextEntry* te, const std::function<void(ley::Command c)>& function); 
};

}
#endif
