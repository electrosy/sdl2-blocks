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

typedef std::map<Uint8, std::pair<ley::Timer, ley::Timer>> InputPressedType;

class InputPressed {

private:
    Uint16 mModifiers;
    ley::Timer mDelayTimer;
    ley::Timer mRepeatTimer;

public:
    Uint16 getModifiers() {return mModifiers;};
    ley::Timer* getDelayTimerPtr() { return &mDelayTimer;};
    ley::Timer* getRepeatTimerPtr() { return &mRepeatTimer;};
    InputPressed(Uint16 sdlKeymod);
};


class Input {

private:
    ley::Command lookupCommand(const Uint8 scancode, std::map<Uint8, ley::Command>* bindings);
    SDL_GameController *mControllerPtr = nullptr;
    std::map<Uint8, std::unique_ptr<InputPressed>> mKeysPressed; //keyboard
    std::map<Uint8, std::pair<ley::Timer, ley::Timer>> mButtonsPressed; //gamepad
    
public:
    Input();
    ~Input();
    ley::Command pollEvents(bool& fullscreen, std::map<Uint8, ley::Command>* buttonBindings2, std::map<Uint8, ley::Command>* bindings2,  std::queue<ley::Command>* commandQueuePtr, ley::TextEntry* te, const std::function<void(ley::Command c)>& function); 
};

}
#endif
