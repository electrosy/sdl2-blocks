#include "../Renderable.h"
#include "RectContainer.h"

#include <map>

#pragma once

namespace ley {

class ProgressBar : public Renderable {

private:
    SDL_Rect border;
    SDL_Rect progress; // Width of progress is updated to fill in only progress area
    SDL_Surface* s; // Surface for the filled in rect which covers the entire progress bar.
    SDL_Texture* t = nullptr;

public:
    ProgressBar(SDL_Rect rect);
    void render(SDL_Renderer* r);
    void adjustProgress(float elapsed, float total);
};

}