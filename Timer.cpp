/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/20/2020
*/
#include "Timer.h"

/* RAII */
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

    if(newTime == 1) {
        SDL_Log("newTime == 1, should not happen"); // TODO this should never happen but for some reason it does 
    }
    else if(mili != newTime && newTime != 0) { //How does newTime ever become 1? Is it a float rounding issue?
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
    if(expired == 1) { 
        expired = 0;
        return 1;
    } else {
        return 0;
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