#include "../Renderable.h"

#include <map>

#pragma once

namespace ley {

class RectContainer : public Renderable {

private:
    std::map<std::string,SDL_Rect> rects;

public:
    RectContainer();
    void operator()(SDL_Renderer*);

    void render(SDL_Renderer* r, bool d);
    void addRect(std::string id, SDL_Rect rect);
};

}