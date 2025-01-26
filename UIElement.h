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

namespace ley {

class UIElement {

private:

    std::string label;
    SDL_Rect source;
    SDL_Rect destination;
    // TODO storing only textures for the elements seem to be messey when updating, maybe we should store font objects instead.
    SDL_Texture* textureBase;
    SDL_Texture* texture;
    SDL_Texture* textureHot;
    bool hot; //indicates that this menu item is currently selected.
    std::function<void()> mUIToggleFunc = {};
    std::function<bool()> mUIInFocus = {};
    std::function<void()> mCommitUI = {};

protected:
   
public:
    UIElement(std::string, SDL_Rect, SDL_Rect, SDL_Texture*, SDL_Texture*, SDL_Texture*);
    UIElement(const std::function<void()> &toggle, const std::function<bool()> &focus, const std::function<void()> &enter);
    void setActiveSelector(bool);
    bool isActiveSelector();
    std::string getLabel();
    SDL_Rect getSource();
    SDL_Rect getDestination();
    SDL_Texture* getTexture();
    SDL_Texture* getTextureHot();
    SDL_Texture* getBase();
    std::function<void()> getFunction() {return mUIToggleFunc;};
    bool getInFocus() {return mUIInFocus();};
    std::function<void()> getEnterFunction() {return mCommitUI;};
    ~UIElement();
};

}

#endif