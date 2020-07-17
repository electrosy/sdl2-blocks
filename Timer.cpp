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

    fill();
}

ley::Timer::~Timer() {

}
/* Functions */
void ley::Timer::fill() {
    SDL_Surface *s;
    s = SDL_CreateRGBSurface(0, rect_border.w, rect_border.h, 32, 0, 0, 0, 0);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 255, 0, 0));

    tex = SDL_CreateTextureFromSurface(renderer,s);
    SDL_RenderCopy(renderer, tex, &rect_tex, &rect_progress);
}
void ley::Timer::reset() {
    clock.reset();
}

void ley::Timer::adjustProgress(float m) {
    destRect_tex.w = ceil(((m/mili)*100));
    fill();
}

void ley::Timer::runFrame(bool autoRestart) {

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