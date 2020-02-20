/* 
Author: Steven Philley
Purpose: A timer works a bit like a stop watch.
         hasElapsed() will return a percentage of the time elapsed.
         hasElapsed() will return 101 if the time has already expired, and will reset the time.
         This object is Renderable.
Date: Feb/20/2020
*/
#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>
#include "SimpleShape.h"

namespace ley {

class Timer : public SimpleShape {

private:
    SDL_Rect rect_border;
    SDL_Rect rect_progress;
    //updateProgress() rect based on time
public:
    Timer(SDL_Renderer*);
    ~Timer();

    void hasElapsed();
    void fill();
    //void runFrame();
};

}
#endif
