/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley. All rights reserved.

Purpose: A timer works a bit like a stop watch.
         This object is Renderable and can render the progress in a renderable status bar.
         A Timer is a SimpleShape which is a Renderable
Date: Feb/20/2020
*/
#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>
#include "./inc/ProgressBar.h"
#include "Renderable.h"
#include "Clock.h"


namespace ley {

class Timer : public Renderable {

private:
    bool sdlTimerReady;
    ProgressBar progressBar;
    float mili; //how many miliseconds this timer runs
    ley::Clock clock;
    bool expired; //expired flag that can get picked up.
    bool mExpiredMessage = true; //is expired state pending to send.
    bool active; //activly running, not paused.
public:
    Timer(int, SDL_Rect rect);
    ~Timer();
    
    void fill(SDL_Renderer * r);
    void runFrame(bool = true, double = 0);
    void reset();
    bool hasExpired();
    bool expiredMessage();
    void setTime(float m); //change time
    int getElapsed();
    double pct(); //percent complete.
    float getSpeed();
    bool isPaused();
    void pause(bool);
    void render(SDL_Renderer* r, bool d);

    void operator()(int, SDL_Rect rect);
};

}
#endif
