/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Jul/14/2020
*/
#include "Font.h"

ley::Font::Font():
mPointSize(DEFAULT_FONT_SIZE) {
    mMessageRect = {0, 0, 0, 0};
    init(mPointSize);
}

ley::Font::Font(int x, int y, int w, int h)
:
mPointSize(DEFAULT_FONT_SIZE) {

    mMessageRect = {x, y, w, h};
    init(mPointSize);
}

ley::Font::Font(const Font& other) {
    
    mPointSize = other.mPointSize;
    init(mPointSize);
    mMessageString = other.mMessageString;
    mMessageRect = other.mMessageRect;
    mColor = other.mColor;
}

void ley::Font::init(int size) {

    mMessageTexture = nullptr;
    mClassic = nullptr;
    mColor = {255, 255, 255, 255};

    if (TTF_Init() < 0) {
        SDL_Log("TTF_Init failed");
    }

    SDL_Log("Open font");
    mClassic = TTF_OpenFont(FONTFILE, size);
    if(!mClassic) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }

}

ley::Font::~Font() {

    cleanUp();
}

void ley::Font::cleanUp() {
    
    if(mMessageTexture) {
        SDL_Log("Destroy font texture");
        SDL_DestroyTexture(mMessageTexture);
        mMessageTexture = nullptr;
    }

    
    if(mClassic) {
        SDL_Log("Close font");
        TTF_CloseFont(mClassic);
        mClassic = nullptr;
    }

    TTF_Quit();
}

//copy assignment operator
ley::Font& ley::Font::operator=(const ley::Font& other) {
    
    cleanUp();
    init(24);
    mMessageRect = other.mMessageRect;
    mMessageString = other.mMessageString;

    return *this;
}

void ley::Font::setColor(SDL_Color c) {
    mColor = c;
}

void ley::Font::updateMessage(std::string s) {
    //Only update the texture if the message has changed.
    if(s != mMessageString) {
        mMessageString = s;

        //then invalidate the texture
        if(mMessageTexture) {
            SDL_DestroyTexture(mMessageTexture);
            mMessageTexture = nullptr;
        }
    }
}

std::string ley::Font::getMessage() {
    return mMessageString;
}

std::string* ley::Font::getMessagePtr() {
    return &mMessageString;
}

SDL_Texture* ley::Font::getTexturePtr() {
    return mMessageTexture;
}

void ley::Font::preRender(SDL_Renderer* r)
{
    if(!mMessageTexture) {
        SDL_Surface* surfaceMessage;
        surfaceMessage = TTF_RenderUTF8_Solid(mClassic, mMessageString.c_str(), mColor);
        mMessageTexture = SDL_CreateTextureFromSurface(r, surfaceMessage);
        SDL_FreeSurface(surfaceMessage);
        surfaceMessage = nullptr;
    }
}

void ley::Font::render(SDL_Renderer * r, bool d) {

    preRender(r);
    
    // TODO this query texture should probably go in the setpos method and be called only when the position changes.
    int w;
    int h;
    SDL_QueryTexture(mMessageTexture,
                     NULL, NULL,
                     &w, &h);
    mMessageRect.h = h;
    mMessageRect.w = w;

    SDL_RenderCopy(r, mMessageTexture, NULL, &mMessageRect);
}

TTF_Font* ley::Font::getTTFFont() {
    return mClassic;
}

void ley::Font::setPos(SDL_Point p) {
    mMessageRect.x = p.x;
    mMessageRect.y = p.y;
}

std::pair<int, int> ley::Font::size() {
    int width = -1;
    int height = -1;

    TTF_SizeText(mClassic, mMessageString.c_str(), &width, &height);

    return std::make_pair(width, height);
}

void ley::Font::setFontSize(int size) {

    mPointSize = size;
    TTF_SetFontSize(mClassic, mPointSize);
}