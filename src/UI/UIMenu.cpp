/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley

Purpose: See header.
Date: Jul/17/2021
*/
#include <string>

#include "../../inc/gfx/Video.h"
#include "../../inc/Input.h"
#include "../../inc/UI/UIMenu.h"
#include "../../inc/Textures.h"

typedef ley::Textures TextureManager;

ley::UIMenu::UIMenu() 
: 
hot(false), 
currentIndex(0),
fader(1000,{0,0,0,0}) {
    fader.reset();
}

ley::UIMenu::~UIMenu() {

    //remove the fonts that are used
    for(auto fontPtr : fontsUsed ) {
        delete fontPtr;
        fontPtr = nullptr;
    }
}

void ley::UIMenu::pushFont(std::string label, const SDL_Rect dest, const std::string s, SDL_Renderer* r, int size) {

    UIElement temp(label, {0,0, dest.w, dest.h}, dest, s, size); // UIElement(std::string l, SDL_Rect sr, SDL_Rect dr, std::string message);
    temp.preRender(r);
    elements.push_back(temp);
}

void ley::UIMenu::pushUIElement(const std::function<void()> &toggle, const std::function<bool()> &focus, const std::function<void()> &enter) {

    UIElement temp(toggle, focus, enter);
    elements.push_back(temp);
}

int ley::UIMenu::count() {
    return elements.size();
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

    return nullptr;
}
ley::UIElement* ley::UIMenu::getCurrentElementPtr() {
    return &elements[currentIndex];
}

int ley::UIMenu::getElementId(std::string label) {
    
    //Iterate through the elements and find the ID that matches the label.
    int index = -1;
    for(int i = 0; i < elements.size(); ++i) {
        if (elements[i].getLabel() == label) {
            index = i;
        }
    }
    return index;
}
void ley::UIMenu::addRenderables(ley::RenderablesPtr r) {
    renderables = r;
}

void ley::UIMenu::renderBaseMenuItems(ley::Video* v) {
    
    //Display all the base menu elements
    for(int i = 0; i < elements.size() && !elements.empty(); ++i) {
        SDL_Rect source = elements[i].getSource();
        SDL_Rect destination = elements[i].getDestination();
        SDL_Texture* baseTexture = elements[i].getBase();

        if(baseTexture) {
            SDL_RenderCopy(v->getRenderer(), baseTexture, &source, &destination);
        }
        else if(elements[i].getBaseFontPtr()->getTexturePtr() == nullptr) {
            // assume this is the font type and pre render the font if its empty.
            elements[i].preRender(v->getRenderer());
        }
        
    }
}

void ley::UIMenu::renderHotItem(ley::Video* v) {
    
    if(elements.size() <= 0) {return; /*EARLY EXIT*/}

    if(elements[currentIndex].getToggleFunction()) {

        if(!elements[currentIndex].getInFocus()) {
            elements[currentIndex].getToggleFunction()();
        }
    }

    SDL_Rect src_rect = currentSrc();
    SDL_Rect dest_rect = currentDest();

    if(SDL_RectEmpty(&src_rect)) {
        return; //Assume we have one of the function pointer type elements.
    }
    
    setHot(false);
    SDL_Texture* mainTexture = currentTex();
    if(!mainTexture) {
        //assume the font type element and pre render the font
        elements[currentIndex].getMainFontPtr()->preRender(v->getRenderer());
    }

    SDL_RenderCopy(v->getRenderer(), mainTexture, &src_rect, &dest_rect);

    setHot(true);
    SDL_Texture* hotTexture = currentTex();
    if(!hotTexture) {
        //assume the font type element and pre render the font
        elements[currentIndex].getHotFontPtr()->preRender(v->getRenderer());
    }
    SDL_SetTextureAlphaMod(hotTexture, mFader.alpha());
    SDL_RenderCopy(v->getRenderer(), hotTexture, &src_rect, &dest_rect);

    mFader.runFrame(); // NOTE This actually goes in update, but this works for now.
}

void ley::UIMenu::render(ley::Video* v) {
    renderBaseMenuItems(v);
    renderHotItem(v);

    renderables.renderAll(v->getRenderer(), false);
}

void ley::UIMenu::runCommand(ley::Command command) {
    if(count() > 0) {
            
        if(command == ley::Command::down || command == ley::Command::right) {

            if(mWidth > 1 && command == ley::Command::down) {
                next(ley::UIMenuItem::row);
            } else {
                next(ley::UIMenuItem::cell);
            }
        }
        
        if(command == ley::Command::cclockwise || command == ley::Command::up || command == ley::Command::left) {
            
            if(mWidth > 1 && (command == ley::Command::up || command == ley::Command::cclockwise)) {
                previous(ley::UIMenuItem::row);
            } else {
                previous(ley::UIMenuItem::cell);
            }
        }
    }
}

int ley::UIMenu::row() {
    return rowAt(currentIndex);
}

int ley::UIMenu::column() {
    return columnAt(currentIndex);
}

int ley::UIMenu::rowAt(int index) {
    int row = -1;
    if(mWidth != 0) {
        row = index / mWidth;
    }

    return row;
}

int ley::UIMenu::columnAt(int index) {
    return index % mWidth;
}

void ley::UIMenu::previous(UIMenuItem item) {
    if(currentIndex > 0) {

        //if we have an active ui element untoggle it before moving to the previous
        if(elements[currentIndex].getToggleFunction()) {
            if(elements[currentIndex].getInFocus()) {
                elements[currentIndex].getToggleFunction()();
            }
            //also run the enter function
            elements[currentIndex].getEnterFunction()();
        }

        if(item == UIMenuItem::cell) {

            if(mWidth > 1) {
                //SDL_Log("col: %i, row: %i", column(), row());
                //if we are at the first column in a row
                if( column() <= 0 ) {
                    
                    return;
                }
            }

            currentIndex--;
        }
        else if (item == UIMenuItem::row) {
            
            int oldCurrentIndex = currentIndex;
            
            currentIndex -= mWidth;

            if(currentIndex < 0) {
                currentIndex = oldCurrentIndex;
            }

            //SDL_Log("col: %i, row: %i", column(), row());
        }
    }
}

void ley::UIMenu::next(UIMenuItem item) {
    if(currentIndex < elements.size()-1) {

        //if we have an active ui element untoggle it before moving to the next
        if(elements[currentIndex].getToggleFunction()) {
            if(elements[currentIndex].getInFocus()) {
                elements[currentIndex].getToggleFunction()();
            }
            //also run the enter function
            elements[currentIndex].getEnterFunction()();
        }

        if(item == UIMenuItem::cell) {

            if(mWidth > 1) {
                //if we are at the last column in a row
                if( column() >= (mWidth - 1) ) {
                    
                    return;
                }
            }

            currentIndex++;
        }
        else if (item == UIMenuItem::row) {
            
            int oldCurrentIndex = currentIndex;
            
            currentIndex += mWidth;

            if(currentIndex > elements.size() - 1) {
                currentIndex = oldCurrentIndex;
            }
        }

        //SDL_Log("col: %i, row: %i", column(), row());
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

void ley::UIMenu::clear() {
    elements.clear();
    currentIndex = 0;
}

ley::UIElement* ley::UIMenu::getElementPtr(std::string label) {

    return &elements.at(getElementId(label));
}