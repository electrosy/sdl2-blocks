/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley

Purpose: see header.
Date: Jul/17/2021
*/
#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <functional>
#include <string>

#include <SDL2/SDL.h>

#include "Font.h"

namespace ley {

class UIElement {

private:

    std::string label;
    std::string mMessage;
    SDL_Rect source;
    SDL_Rect destination;
    ley::Font mBaseFont;
    ley::Font mHotFont;
    ley::Font mMainFont;
    bool hot; //indicates that this menu item is currently selected.
    std::function<void()> mUIToggleFunc = {};
    std::function<bool()> mUIInFocus = {};
    std::function<void()> mCommitUI = {};
    void setDimensions();

protected:
   
public:
    UIElement(std::string l, SDL_Rect sr, SDL_Rect dr, std::string message);
    UIElement(const std::function<void()> &toggle, const std::function<bool()> &focus, const std::function<void()> &enter);
    void setActiveSelector(bool);
    bool isActiveSelector();
    std::string getLabel();
    SDL_Rect getSource();
    SDL_Rect getDestination();
    SDL_Texture* getTexture();
    SDL_Texture* getTextureHot();
    SDL_Texture* getBase();
    ley::Font* getBaseFontPtr() { return &mBaseFont; };
    ley::Font* getMainFontPtr() { return &mMainFont; };
    ley::Font* getHotFontPtr() { return &mHotFont; };
    void setMessage(std::string message);
    void preRender(SDL_Renderer* r);
    std::function<void()> getFunction() {return mUIToggleFunc;};
    bool getInFocus() {return mUIInFocus();};
    std::function<void()> getEnterFunction() {return mCommitUI;};
    ~UIElement();
};

}

#endif