/* 
sdl2-blocks
Copyright (C) 2020-2021 Steven Philley. All rights reserved.

Purpose: see header.
Date: Feb/16/2020
*/
#include "Clock.h"

/* RAII */
ley::Clock::Clock() 
: 
pausestart(0), 
active(true), 
pauseend(0) {
    
}

ley::Clock::~Clock() {

}

/* Functions */
void ley::Clock::pause(bool p) {

    if(active) {
        pausestart = SDL_GetTicks();
    } else {
        pauseend = SDL_GetTicks();
    }

    active = !p;

    if(!p) {

        start = start + (pauseend - pausestart);
        pausestart = 0;
        pauseend = 0;
    }
}

Uint32 ley::Clock::secondsFromStart() {
    return miliSecondsFromStart()/1000;
}
// TODO generalzie these functions to pass in the time increment
Uint32 ley::Clock::miliSecondsFromStart() {
    auto end = SDL_GetTicks();

    return end - start;
}

void ley::Clock::reset() {
    start = SDL_GetTicks();
}
