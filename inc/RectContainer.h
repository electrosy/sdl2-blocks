#pragma once

#include "./gfx/Renderable.h"
#include <unordered_map>

namespace ley {

class RectContainer : public Renderable {

private:
    std::unordered_map<std::string, SDL_Rect> rects;
    unsigned char mFillBackgroundTransparency;

public:
    RectContainer();
    void operator()(SDL_Renderer*);

    void render(SDL_Renderer* r, bool d);
    void addRect(const std::string& id, SDL_Rect rect);
    SDL_Rect* getRect(const std::string& id);
    void increaseTransparency();
    void decreaseTransparency();
};

}
