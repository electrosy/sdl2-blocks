/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/16/2020
*/
#include "Clock.h"

/* RAII */
ley::Clock::Clock() {
    
}

ley::Clock::~Clock() {

}

/* Functions */
Uint32 ley::Clock::secondsFromStart() {
    auto end = SDL_GetTicks();
    return end - start;
}
// TODO generalzie these functions to pass in the time increment
Uint32 ley::Clock::miliSecondsFromStart() {
    auto end = SDL_GetTicks(); 
    return end - start;
}

void ley::Clock::reset() {
    start = SDL_GetTicks();
}
