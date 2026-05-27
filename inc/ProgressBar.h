#pragma once

#include "./gfx/Renderable.h"
#include "Timer.h"

#include <map>

namespace ley {

class ProgressBar : public Renderable {

private:
    SDL_Rect border;
    SDL_Rect progress; // Width updated to fill only the progress area
    Timer* mTimer = nullptr; // optional: if set, adjustProgress() is driven automatically in render()

public:
    // rect-only: caller drives adjustProgress() manually
    ProgressBar(SDL_Rect rect);
    // timer-linked: render() automatically syncs progress from the timer
    ProgressBar(SDL_Rect rect, Timer* timer);

    void operator()(SDL_Rect rect);   // re-initialise rect (timer link preserved)
    void init();
    void render(SDL_Renderer* r, bool d);
    void adjustProgress(float elapsed, float total);
};

}
