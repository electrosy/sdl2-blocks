/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/20/2020
*/
#include "Timer.h"

/* RAII */
ley::Timer::Timer() 
: Renderable(),
mili(1000), 
active(true),
expired(false),
sdlTimerReady(true),
progressBar({0,0,0,0}) {

    if(SDL_Init(SDL_INIT_TIMER) >= 0) {
        // if all good
    } else {
        printf("Can't Initialize SDL2 Timer");
        sdlTimerReady = 0;
    }
}

ley::Timer::Timer(int m, SDL_Rect rect) 
: Renderable(),
mili(m),
active(true), 
expired(false), 
sdlTimerReady(true),
progressBar{rect} {
    
    if(SDL_Init(SDL_INIT_TIMER) >= 0) {
        // if all good
    } else {
        printf("Can't Initialize SDL2 Timer");
        sdlTimerReady = 0;
    }
}
void ley::Timer::operator()(int i, SDL_Rect rect) {
    mili = i;
    progressBar(rect);
}
ley::Timer::~Timer() {
    if(sdlTimerReady && SDL_InitSubSystem(SDL_INIT_TIMER)) {
        SDL_QuitSubSystem(SDL_INIT_TIMER);
    }
}

//copy assignment operator
ley::Timer& ley::Timer::operator=(ley::Timer other) {

    sdlTimerReady = other.sdlTimerReady;
    progressBar = other.progressBar;
    mili = other.mili;
    clock = other.clock;
    expired = other.expired;
    mExpiredMessage = other.mExpiredMessage;
    active = other.active;

    return *this;
}


/* Functions */
void ley::Timer::reset() {

    if(!isPaused()) {
        clock.reset();
    }

    mExpiredMessage = true;
}

void ley::Timer::runFrame(bool autoRestart, double newTime) {

    if(!active) {
        return; //timer is paused.
    }

    if(mili != newTime && newTime != 0) {
        mili = newTime;
    }

    unsigned int miliFromStart = clock.miliSecondsFromStart();
    if(miliFromStart > mili) {
        expired = 1;
       if(autoRestart) { reset(); }
    }
    
    progressBar.adjustProgress(miliFromStart, mili);
}

bool ley::Timer::hasExpired() {
    
    //TODO it doesn't appear that this works as a message pass because expired continually gets set back to true anyway.
    if(expired) { 
        expired = false;
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
    
    if(getElapsed() > mili) {
        return 1; //Never return more than 100%;
    }
    else if(mili != 0) {
        return getElapsed() / mili;
    }
    else {
        return 1;
    }
} 
void ley::Timer::setTime(float m) {
    mili = m;
}

int ley::Timer::getElapsed() {
    return clock.miliSecondsFromStart();
}

float ley::Timer::getSpeed() {
    return mili;
}

bool ley::Timer::isPaused() {
    return !active;
}

void ley::Timer::pause(bool paused) {
    clock.pause(paused);
    active = !paused;
}

void ley::Timer::render(SDL_Renderer* r, bool d) {
    progressBar.render(r, d);
}