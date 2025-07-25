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
#include "ProgressBar.h"
#include "./gfx/Renderable.h"
#include "Clock.h"

namespace ley {

// TODO a timer should not be a renderable.
class Timer : public Renderable {

private:
    bool mSDLTimerReady;
    ProgressBar mProgressBar; // TODO rip the progress bar out of timer. a ProgressBar should have a timer.
    float mMili; //how many miliseconds this timer runs
    Clock mClock;
    bool mExpired; //expired flag that can get picked up.
    bool mExpiredMessage = true; //is expired state pending to send.
    bool mActive; //activly running, not paused.
public:
    /* RAII */
    Timer();
    Timer(int, SDL_Rect rect);
    ~Timer();
    /* Operators */
    Timer& operator=(Timer other); //copy assignment
    /* Accessors */
    void setTime(float m);
    int getElapsed();
    bool hasExpired();
    float getSpeed();
    bool isPaused();
    void pause(bool);
    /* Functions */
    void render(SDL_Renderer* r, bool d);
    void runFrame(bool = true, double = 0);
    bool expiredMessage();
    double pct(); //percent complete.
    void operator()(int, SDL_Rect rect);
    void expire(); //force and experation of the timer
    void reset();

    
};

}
#endif
