/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: Contains font information
Date: Jul/14/2020
*/
#ifndef FONT_H
#define FONT_H
#include <SDL2/SDL_ttf.h>

#include "Renderable.h"

namespace ley {

class Font : public Renderable {

private:
    std::string textMessage;
    SDL_Surface* surfaceMessage;
    SDL_Texture* Message;
    SDL_Rect Message_rect;
    TTF_Font* Classic;
    void updateTexture();
    SDL_Color White = {255, 255, 255};

protected:

public:
    Font(SDL_Renderer* r, int = 0, int = 0);
    ~Font();
    void updateMessage(std::string);
    void render();

};

}
#endif