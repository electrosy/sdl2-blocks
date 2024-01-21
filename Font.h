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
    SDL_Texture* Message;
    SDL_Rect Message_rect;
    TTF_Font* Classic;
    SDL_Color White = {255, 255, 255};
    
protected:

public:
    
    Font(int = 0, int = 0, int = 0, int = 0);
    ~Font();
    Font& operator=(Font other); //copy assignment
    Font operator()(Font& other); //copy constructor
    
    void updateMessage(std::string s);
    std::string getMessage();
    std::string* getMessagePtr();
    void render(SDL_Renderer * r, bool d);
    TTF_Font* getTTFFont();
    void setPos(SDL_Point p);
    
};

}
#endif