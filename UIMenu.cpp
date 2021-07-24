/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley

Purpose: See header.
Date: Jul/17/2021
*/
#include <string>

#include "UIMenu.h"
#include "UIElement.h"

ley::UIMenu::UIMenu() 
: hot(false), currentIndex(0) {
    
}

ley::UIMenu::~UIMenu() {

}
void ley::UIMenu::push(std::string label, const SDL_Rect src, const SDL_Rect dest, SDL_Texture* base, SDL_Texture* tex, SDL_Texture* texhot) {
    //push a UI Element into the UI Menu
    UIElement temp(label, src, dest, base, tex, texhot);
    elements.push_back(temp);
}
void ley::UIMenu::setHot(bool h) {
    hot = h;
}
SDL_Texture* ley::UIMenu::currentTex() {
    if(hot) {
        return elements.at(currentIndex).getTextureHot();
    }
    else {
        return elements.at(currentIndex).getTexture();
    }
}

void ley::UIMenu::getBaseElements(std::vector< std::tuple<SDL_Rect, SDL_Rect, SDL_Texture*> > *baseElements) {
    //Iterate through all elements and return only the base elements.

    for(int i = 0; i < elements.size() && !elements.empty(); ++i) {
        
        baseElements->push_back(   std::make_tuple( 
                                    elements.at(i).getSource(), elements.at(i).getDestination(), elements.at(i).getBase()
                                    ));
    }
}

void ley::UIMenu::previous() {
    if(currentIndex > 0) {
        currentIndex--;
    }
}

void ley::UIMenu::next() {

    if(currentIndex < elements.size()-1) {
        currentIndex++;
    }
}

SDL_Rect ley::UIMenu::currentDest() {
    return elements.at(currentIndex).getDestination();
}

SDL_Rect ley::UIMenu::currentSrc() {
    return elements.at(currentIndex).getSource();
}

int ley::UIMenu::getIndex() {
    return currentIndex;
}