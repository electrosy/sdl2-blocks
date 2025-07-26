/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: Input System for the game.
Date: Feb/14/2020
*/

#ifndef INPUT_H
#define INPUT_H


#include <map>
#include <vector>
#include <string>
#include <queue>

#include <SDL2/SDL.h>

#include <functional>
#include "UI/TextEntry.h"
#include "Command.h"



namespace ley {

const auto KEY_DELAY_TIME_DEFAULT = 250;
const auto KEY_REPEAT_TIME_DEFAULT = 35;

typedef std::map<Uint8, std::pair<ley::Timer, ley::Timer>> InputPressedType;
typedef std::multimap<SDL_Scancode, std::pair<Uint16, ley::Command>> KeyBindingsType; //SDL_Scancode, <Modifers, Command>
typedef std::map<SDL_GameControllerButton, ley::Command> PadBindingsType; //SDL_GameControllerButton, <Modifers, Command>

class InputPressed {

private:
    Uint16 mModifiers;
    ley::Timer mDelayTimer;
    ley::Timer mRepeatTimer;

public:
    Uint16 getModifiers() {return mModifiers;};
    ley::Timer* getDelayTimerPtr() { return &mDelayTimer;};
    ley::Timer* getRepeatTimerPtr() { return &mRepeatTimer;};
    InputPressed(Uint16 sdlKeymod, Uint16 delayTime, Uint16 repeatTime); 
};


class Input {

private:

    // TODO lookupCommand should be replaced with lookupCommand2 and renamed to lookupCommand.
    ley::Command lookupPadCommand(const SDL_GameControllerButton scancode, PadBindingsType* bindings);
    ley::Command lookupKeyCommand(const SDL_Scancode scancode, Uint16 modifiers, KeyBindingsType* bindings);
    SDL_GameController *mControllerPtr = nullptr; //For the gamepad.
    std::map<Uint8, std::unique_ptr<InputPressed>> mKeysPressed; //keyboard
    std::map<Uint8, std::pair<ley::Timer, ley::Timer>> mButtonsPressed; //gamepad
    
public:
    Input();
    ~Input();
    void pollEvents(bool& fullscreen, 
        PadBindingsType* buttonBindings2,
        KeyBindingsType* bindingsNewType, 
        std::queue<ley::Command>* commandQueuePtr, 
        ley::UIWidget* te, 
        const std::function<void(ley::Command c)>& function, 
        int keyDelay,
        int keyRepeat);
};

}
#endif

