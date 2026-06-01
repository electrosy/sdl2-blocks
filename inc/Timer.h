/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley. All rights reserved.

Purpose: A timer works like a stopwatch — pure timing, no rendering.
Date: Feb/20/2020
*/
#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>
#include "Clock.h"

namespace ley {

class Timer {

private:
    bool mSDLTimerReady;
    float mMili; //how many milliseconds this timer runs
    Clock mClock;
    bool mExpired;           //expired flag that can get picked up
    bool mExpiredMessage = true; //is expired state pending to send
    bool mActive;            //actively running, not paused

public:
    /* RAII */
    Timer();
    Timer(int ms);
    ~Timer();
    /* Operators */
    Timer& operator=(Timer other); //copy assignment
    void operator()(int ms);       //re-initialise duration
    /* Accessors */
    void setTime(float m);
    int getElapsed();
    bool hasExpired();
    float getSpeed();
    bool isPaused() const;
    void pause(bool);
    /* Functions */
    void runFrame(bool = true, double = 0);
    bool expiredMessage();
    double pct(); //percent complete
    void expire(); //force an expiration of the timer
    void reset();
};

}
#endif
