/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/20/2020
*/
#include "Timer.h"

/* RAII */
ley::Timer::Timer(SDL_Renderer* r, unsigned int m, SDL_Rect rect)
: SimpleShape(r), mili(m), rect_border(rect), rect_progress(rect) {
    
    rect_tex.x = 0; rect_tex.y = 0; //rectange for internal texture
    rect_tex.w = rect_border.w;
    rect_tex.h = rect_border.h;
    addShape("timer-border", rect_border);
    addShape("timer-progress", rect_progress);
    destRect_tex.x=rect_progress.x; destRect_tex.y=rect_progress.y;
    destRect_tex.w=rect_progress.w; destRect_tex.h=rect_progress.h;

    s = SDL_CreateRGBSurface(0, rect_border.w, rect_border.h, 32, 0, 0, 0, 0);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 255, 0, 0));
    tex = SDL_CreateTextureFromSurface(renderer,s);

    fill();
}

ley::Timer::~Timer() {
    SDL_FreeSurface(s);
}
/* Functions */
void ley::Timer::fill() {

    SDL_RenderCopy(renderer, tex, &rect_tex, &rect_progress);
    
}
void ley::Timer::reset() {
    clock.reset();
}

void ley::Timer::adjustProgress(float m) {

    if(mili != 0) {
        destRect_tex.w = ceil(((m/mili)*rect_progress.w));
    } else {
        destRect_tex.w = 0;
    }
    
    fill();
}

void ley::Timer::runFrame(bool autoRestart, double newTime) {

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
    adjustProgress(miliFromStart);
}

bool ley::Timer::hasExpired() {
    
    if(expired == 1) {
        expired = 0;
        return 1;
    } else {
        return 0;
    }

}

int ley::Timer::getElapsed() {
    return clock.miliSecondsFromStart();
}

void ley::Timer::changeSpeed(float newSpeed) {

   // mili = newSpeed; //causes a set fault, maybe the timer needs to be recreated completly.
}

float ley::Timer::getSpeed() {
    return mili;
}