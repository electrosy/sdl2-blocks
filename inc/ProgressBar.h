#include "../Renderable.h"
#include "RectContainer.h"

#include <map>

#pragma once

// NiceToHave: Instead of solid red for the progress bar a more creative rainbow type faded colors could be used.

namespace ley {

class ProgressBar : public Renderable {

private:
    SDL_Rect border;
    SDL_Rect progress; // Width of progress is updated to fill in only progress area
public:
    ProgressBar(SDL_Rect rect);
    ProgressBar(const ProgressBar &pb) = delete;
    void operator()(SDL_Rect rect);
    void init();
    void render(SDL_Renderer* r, bool d);
    void adjustProgress(float elapsed, float total);
};

}