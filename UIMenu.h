/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley

Purpose: A Collection of UIElements
Date: Jul/17/2021
*/
#ifndef UIMENU_H
#define UIMENU_H

#include <vector>
#include <tuple>

#include <SDL2/SDL.h>

#include "UIElement.h"
#include "Textures.h"

//#typedef std::tuple<std::string, SDL_Rect, SDL_Rect, SDL_Texture*> menuelement;

enum class menutypes {main,options,highscore};

namespace ley {

class UIMenu {

private:
    
    //std::tuple<std::string, SDL_Rect, SDL_Rect, SDL_Texture*> menuelement;
    //std::vector<menuelement> numbers;
    std::vector<ley::UIElement> elements;

/*
    SDL_Rect source;
    SDL_Rect destination;
    SDL_Texture* texture;
    SDL_Texture* textureHot;
    */
    int currentIndex;
    bool hot; //indicates that this menu item is currently selected.

protected:
   
public:
    UIMenu();
    ~UIMenu();

    void next();
    void previous();
    SDL_Texture* currentTex();
    SDL_Rect currentSrc();
    SDL_Rect currentDest();
    void setHot(bool);
    void push(std::string, const SDL_Rect, const SDL_Rect, std::string, std::string, std::string); //WStrings (new way)
    int getIndex();
    void getBaseElements(std::vector< std::tuple<SDL_Rect, SDL_Rect, SDL_Texture*>> *baseElements);
    void clear(); //clear out all the elements.
    int runMenu(ley::Video*, ley::Input*, ley::GameModel*, bool, std::string, SDL_Rect, double, menutypes);

};

}

#endif