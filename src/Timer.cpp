/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/20/2020
*/
#include "../inc/Timer.h"

/* RAII */
ley::Timer::Timer() 
: Renderable(),
mMili(1000), 
mActive(true),
mExpired(false),
mSDLTimerReady(true),
mProgressBar({0,0,0,0}) {

    if(SDL_Init(SDL_INIT_TIMER) >= 0) {
        // if all good
    } else {
        printf("Can't Initialize SDL2 Timer");
        mSDLTimerReady = 0;
    }
}

ley::Timer::Timer(int m, SDL_Rect rect) 
: Renderable(),
mMili(m),
mActive(true), 
mExpired(false), 
mSDLTimerReady(true),
mProgressBar{rect} {
    
    if(SDL_Init(SDL_INIT_TIMER) >= 0) {
        // if all good
    } else {
        printf("Can't Initialize SDL2 Timer");
        mSDLTimerReady = 0;
    }
}
void ley::Timer::operator()(int i, SDL_Rect rect) {
    mMili = i;
    mProgressBar(rect);
}
ley::Timer::~Timer() {
    if(mSDLTimerReady && SDL_InitSubSystem(SDL_INIT_TIMER)) {
        SDL_QuitSubSystem(SDL_INIT_TIMER);
    }
}
/* Operators */
//copy assignment operator
ley::Timer& ley::Timer::operator=(ley::Timer other) {

    mSDLTimerReady = other.mSDLTimerReady;
    mProgressBar = other.mProgressBar;
    mMili = other.mMili;
    mClock = other.mClock;
    mExpired = other.mExpired;
    mExpiredMessage = other.mExpiredMessage;
    mActive = other.mActive;

    return *this;
}

/* Functions */
void ley::Timer::reset() {

    if(!isPaused()) {
        mClock.reset();
    }

    mExpiredMessage = true;
}

void ley::Timer::runFrame(bool autoRestart, double newTime) {

    if(!mActive) {
        return; //timer is paused.
    }

    if(mMili != newTime && newTime != 0) {
        mMili = newTime;
    }

    unsigned int miliFromStart = mClock.miliSecondsFromStart();
    if(miliFromStart > mMili) {
        mExpired = 1;
       if(autoRestart) { reset(); }
    }
    
    mProgressBar.adjustProgress(miliFromStart, mMili);
}

void ley::Timer::expire() {
    
}

bool ley::Timer::hasExpired() {
    
    //TODO it doesn't appear that this works as a message pass because expired continually gets set back to true anyway.
    if(mExpired) { 
        mExpired = false;
        return true;
    } else {
        return false;
    }
}
bool ley::Timer::expiredMessage() {
    
    bool returnValue = false;

    if(mExpiredMessage && hasExpired()) {
        returnValue = true;
        mExpiredMessage = false;
    }

    return returnValue;
}
double ley::Timer::pct() {
    
    if(getElapsed() > mMili) {
        return 1; //Never return more than 100%;
    }
    else if(mMili != 0) {
        return getElapsed() / mMili;
    }
    else {
        return 1;
    }
} 

void ley::Timer::render(SDL_Renderer* r, bool d) {
    mProgressBar.render(r, d);
}

/* Accessors */
void ley::Timer::setTime(float m) {
    mMili = m;
}

int ley::Timer::getElapsed() {
    return mClock.miliSecondsFromStart();
}

float ley::Timer::getSpeed() {
    return mMili;
}

bool ley::Timer::isPaused() {
    return !mActive;
}

void ley::Timer::pause(bool paused) {
    mClock.pause(paused);
    mActive = !paused;
}