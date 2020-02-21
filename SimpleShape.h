/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A SimpleShape is a Renderable object that deals with rendering Pixels, Rects, Lines, etc...
Date: Feb/20/2020
*/
#ifndef SIMPLESHAPE_H
#define SIMPLESHAPE_H

#include <map>
#include "Renderable.h"

namespace ley {

class SimpleShape : public Renderable {

private:
    std::map<std::string,SDL_Rect> shapes;
    void drawRect(const SDL_Rect*);
protected:
    SDL_Texture *tex;
    SDL_Rect rect_tex;
    SDL_Rect destRect_tex;
public:
    SimpleShape(SDL_Renderer*);
    ~SimpleShape();

    void addShape(std::string, SDL_Rect);
    void render();
};

}
#endif
