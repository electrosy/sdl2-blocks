/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley. All rights reserved.

Purpose: A Collection of UIElements
Date: Jul/17/2021
*/
#ifndef UIMENU_H
#define UIMENU_H

#include <vector>
#include <tuple>
#include <list>
#include <SDL2/SDL.h>
#include "UIElement.h"
#include "Textures.h"
#include "Renderables.h"

namespace ley {

class UIMenu {

private:

    std::vector<ley::UIElement> elements;
    std::multimap<std::string,ley::UIElement> selectors; //element id, and selectors
    int currentIndex;
    bool hot; //indicates that this menu item is currently selected.
    ley::Renderables renderables;

protected:
   
public:

    UIMenu();
    ~UIMenu();

    void next();
    void previous();
    void toggle();
    SDL_Texture* currentTex();
    SDL_Rect currentSrc();
    SDL_Rect currentDest();
    void setHot(bool);
    void push(std::string, const SDL_Rect, const SDL_Rect, const std::string, const std::string, const std::string);
    int getIndex();
    void getBaseElements(std::vector< std::tuple<SDL_Rect, SDL_Rect, SDL_Texture*>> *baseElements);
    void clear(); //clear out all the elements.
    // TODO remove runMenu
    /*int runMenu(ley::Video*, ley::Input*, ley::GameModel*, std::string, SDL_Rect, double);*/
    int count();
    void addRenderables(ley::Renderables);
    void addSelector(std::string, const SDL_Rect, const SDL_Rect, const std::string, const std::string, const std::string);
    int getElementId(std::string);
    void runIntroScreen(ley::Video*, ley::Input*, ley::GameModel*, std::string, SDL_Rect, double);
    
    void renderBaseMenuItems(ley::Video* v);
    void renderHotItem(ley::Video* v);
    void renderSelectors(ley::Video* v);
    void render(ley::Video* v);
    void runCommand(ley::Command command);

};

}

#endif