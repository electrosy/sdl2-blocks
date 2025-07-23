/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A system to deal with timers and retrieving the system time.
Date: Feb/16/2020
*/
#ifndef CLOCK_H
#define CLOCK_H

#include <string>

#include <SDL2/SDL.h>


namespace ley {

class Clock {

private:
    Uint32 start = SDL_GetTicks();
    Uint32 pausestart;
    Uint32 pauseend;
    bool active;

public:
    Clock();
    ~Clock();

    Uint32 secondsFromStart();
    Uint32 miliSecondsFromStart();
    void reset();
    void pause(bool);
};

}
#endif
