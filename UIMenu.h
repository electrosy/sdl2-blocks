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
#include "Textures.h"
#include "Renderables.h"
#include "Timer.h"
#include "./inc/Fader.h"

namespace ley {

class UIMenu {

private:

    std::vector<ley::UIElement> elements; // TODO elements should probably be in a map with label for key and UIElement for value.
    std::vector<ley::Font*> fontsUsed;
    int currentIndex;
    bool hot; //indicates that this menu item is currently selected.
    ley::Renderables renderables;
    ley::Timer fader;
    ley::Fader mFader;
    char mFaderControl = 0; //0 = fade in, 1 = fade out, 2 = hold //Run fade in reverse from in to out instead of out to in.

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
    void pushFont(std::string label, const SDL_Rect dest, const std::string s, SDL_Renderer* r);
    void pushTextEntry(const std::function<void()> &toggle, const std::function<bool()> &focus, const std::function<void()> &enter);
    int getIndex();
    void getBaseElements(std::vector< std::tuple<SDL_Rect, SDL_Rect, SDL_Texture*>> *baseElements);
    void clear(); //clear out all the elements.
    int count();
    void addRenderables(ley::Renderables);
    int getElementId(std::string);
    ley::UIElement* getElementPtr(std::string label); //Get elementPtr from label
    void renderBaseMenuItems(ley::Video* v);
    void renderHotItem(ley::Video* v);
    void render(ley::Video* v);
    void runCommand(ley::Command command);
};

}

#endif