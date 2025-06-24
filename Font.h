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

const auto DEFAULT_FONT_SIZE = 24;

class Font : public Renderable {

private:
    std::string mMessageString;
    SDL_Texture* mMessageTexture;
    SDL_Rect mMessageRect;
    TTF_Font* mTTFFont;
    SDL_Color mColor;
    int mPointSize;

protected:

public:
    Font();
    Font(int, int, int, int);
    Font(const Font& other); //copy constructor
    ~Font();
    void cleanUp();
    Font& operator=(const Font& other); //copy assignment
    void init(int size);
    void updateMessage(std::string s);
    std::string getMessage();
    std::string* getMessagePtr();
    SDL_Texture* getTexturePtr();
    void preRender(SDL_Renderer* r);
    void render(SDL_Renderer * r, bool d);
    TTF_Font* getTTFFont();
    void setX(int x);
    void setY(int Y);
    void setPos(SDL_Point p);
    SDL_Point getPos() { return {mMessageRect.x, mMessageRect.y}; };
    std::pair<int, int> size();
    void setColor(SDL_Color c);
    void setFontSize(int size);
    void center();
    void bottom(int screenHeight); //move the font to the bottom of the screen
    void left();
    void right(int screenWidth); //move the font to the right of the screen
};

}
#endif