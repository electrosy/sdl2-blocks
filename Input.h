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



namespace ley {

enum class Command {
    down,
    right,
    up,
    left,
    none,
    pause,
    space,
    console,
    cclockwise,
    clockwise,
    debugkeystoggle,
    debugcolide,
    debugtexture,
    debugnextlevel,
    debugprevlevel,
    debugclear,
    debugfill,
    debugonlyline,
    enter,
    quit,
    increaseVolume,
    decreaseVolume,
    nextSong,
    previousSong,
    tab,
    backspace}; //these include directions and other inputs, this is overloaded maybe a couple enums would work better?

enum class Character {
    backspace,enter,none
};

class Input {

private:
  
public:
    Input();
    ~Input();
    // TODO we may not need to have all these different function,
    // there is likely a way to generalize them.
    ley::Command pollEvents(bool& fullscreen, /*ley::GameStateMachine* gsm*/std::string* ptr_s, const std::function<void(ley::Command c)>& function);
    void pollTextEvents(std::string* ptr_s, Sint32 cursor, Sint32 selection_len);
    ley::Command pollTitleEvents(bool&);

};

}
#endif
