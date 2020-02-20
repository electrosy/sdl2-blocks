/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/20/2020
*/
#include "Timer.h"

/* RAII */
ley::Timer::Timer(SDL_Renderer* r) 
: SimpleShape(r) {
    
    rect_border.x = 300; rect_border.y = 300;
    rect_border.w = 100;
    rect_border.h = 25;

    rect_tex.x = 0; rect_tex.y = 0; //rectange for internal texture
    rect_tex.w = rect_border.w;
    rect_tex.h = rect_border.h;
    
    rect_progress.x = rect_border.x; rect_progress.y = rect_border.y;
    rect_progress.w = rect_border.w; rect_progress.h = rect_border.h;
    addShape("timer-border",rect_border);
    addShape("timer-progress", rect_progress);
}

ley::Timer::~Timer() {

}
/* Functions */
void ley::Timer::fill() {
    SDL_Surface *s;
    s = SDL_CreateRGBSurface(0, rect_border.w, rect_border.h, 32, 0, 0, 0, 0);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 255, 0, 0));

    tex = SDL_CreateTextureFromSurface(renderer,s);
    SDL_RenderCopy(renderer, tex, &rect_tex, &rect_border);
}