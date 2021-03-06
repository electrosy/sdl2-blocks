/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A timer works a bit like a stop watch.
         hasElapsed() will return a percentage of the time elapsed.
         hasElapsed() will return 101 if the time has already expired, and will reset the time.
         This object is Renderable.
Date: Feb/20/2020
*/
#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>
#include "SimpleShape.h"
#include "Clock.h"

namespace ley {

class Timer : public SimpleShape {

private:
    SDL_Rect rect_border;
    SDL_Rect rect_progress;
    float mili; //how many miliseconds this timer runs
    ley::Clock clock;
    bool expired; //expired flag that can get picked up.
    //updateProgress() rect based on time
    void adjustProgress(float);

    SDL_Surface *s;
public:
    Timer(SDL_Renderer*, unsigned int, SDL_Rect rect);
    ~Timer();

    void hasElapsed();
    void fill();
    void runFrame(bool = true, double = 0);
    void reset();
    bool hasExpired();
    int getElapsed();
    void changeSpeed(float);
    float getSpeed();
};

}
#endif
