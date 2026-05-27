/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/20/2020
*/
#include "../inc/Timer.h"

/* RAII */
ley::Timer::Timer()
: mMili(1000),
  mActive(true),
  mExpired(false),
  mSDLTimerReady(true) {

    if(SDL_Init(SDL_INIT_TIMER) < 0) {
        printf("Can't Initialize SDL2 Timer");
        mSDLTimerReady = false;
    }
}

ley::Timer::Timer(int ms)
: mMili(ms),
  mActive(true),
  mExpired(false),
  mSDLTimerReady(true) {

    if(SDL_Init(SDL_INIT_TIMER) < 0) {
        printf("Can't Initialize SDL2 Timer");
        mSDLTimerReady = false;
    }
}

ley::Timer::~Timer() {
    if(mSDLTimerReady && SDL_InitSubSystem(SDL_INIT_TIMER)) {
        SDL_QuitSubSystem(SDL_INIT_TIMER);
    }
}

/* Operators */
ley::Timer& ley::Timer::operator=(ley::Timer other) {
    mSDLTimerReady  = other.mSDLTimerReady;
    mMili           = other.mMili;
    mClock          = other.mClock;
    mExpired        = other.mExpired;
    mExpiredMessage = other.mExpiredMessage;
    mActive         = other.mActive;
    return *this;
}

void ley::Timer::operator()(int ms) {
    mMili = ms;
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
        return; //timer is paused
    }

    if(mMili != newTime && newTime != 0) {
        mMili = newTime;
    }

    unsigned int miliFromStart = mClock.miliSecondsFromStart();
    if(miliFromStart > mMili) {
        mExpired = true;
        if(autoRestart) { reset(); }
    }
}

void ley::Timer::expire() {
    // intentionally empty — force-expire hook
}

bool ley::Timer::hasExpired() {
    if(mExpired) {
        mExpired = false;
        return true;
    }
    return false;
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
        return 1; // Never return more than 100%
    }
    else if(mMili != 0) {
        return getElapsed() / mMili;
    }
    return 1;
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
