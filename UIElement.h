/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley

Purpose: see header.
Date: Jul/17/2021
*/
#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <string>

#include <SDL2/SDL.h>

namespace ley {

class UIElement {

private:

    std::string label;
    SDL_Rect source;
    SDL_Rect destination;
    SDL_Texture* textureBase;
    SDL_Texture* texture;
    SDL_Texture* textureHot;
    bool hot; //indicates that this menu item is currently selected.

protected:
   
public:
    UIElement(std::string, SDL_Rect, SDL_Rect, SDL_Texture*, SDL_Texture*, SDL_Texture*);
    std::string getLabel();
    SDL_Rect getSource();
    SDL_Rect getDestination();
    SDL_Texture* getTexture();
    SDL_Texture* getTextureHot();
    SDL_Texture* getBase();
    ~UIElement();
};

}

#endif