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

const auto FONTFILE = "assets/fonts/MartianMono-Regular.ttf";

class Font : public Renderable {

private:
    std::string mMessageString;
    SDL_Texture* mMessageTexture;
    SDL_Rect mMessageRect;
    TTF_Font* mClassic;
    SDL_Color mColor;

protected:

public:
    Font();
    Font(int, int, int, int);
    Font(const Font& other);
    ~Font();
    void cleanUp();
    Font& operator=(const Font& other); //copy assignment
    //Font operator()(Font& other); //copy constructor
    void init();
    void updateMessage(std::string s);
    std::string getMessage();
    std::string* getMessagePtr();
    SDL_Texture* getTexturePtr();
    void preRender(SDL_Renderer* r);
    void render(SDL_Renderer * r, bool d);
    TTF_Font* getTTFFont();
    void setPos(SDL_Point p);
    std::pair<int, int> size();
    void setColor(SDL_Color c);
};

}
#endif