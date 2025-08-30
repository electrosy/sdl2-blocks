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
mCurrentIndex(0) {
    
}

ley::UIMenu::~UIMenu() {

    //remove the fonts that are used
    for(auto fontPtr : mFontsUsed ) {
        delete fontPtr;
        fontPtr = nullptr;
    }
}
void ley::UIMenu::pushPlaceHolder() {
    mElements.push_back({});
}

void ley::UIMenu::pushFont(std::string elementid, ley::Font* inFont, SDL_Renderer* r) {
    
    int w = 0;
    int h = 0;
    TTF_SizeUTF8(inFont->getTTFFont() , inFont->getMessage().c_str(), &w, &h);
    
    SDL_Point pos = inFont->getPos();
    pushFont(elementid, {pos.x,pos.y,w,h}, inFont->getMessage(), r, inFont->getFontSize());
}

// TODO this uses only the width and height from the SDL_Rect, switch to an SDL_Point or something.
void ley::UIMenu::pushFont(std::string label, const SDL_Rect dest, const std::string s, SDL_Renderer* r, int size) {

    UIElement temp(label, {0,0, dest.w, dest.h}, dest, s, size); // UIElement(std::string l, SDL_Rect sr, SDL_Rect dr, std::string message);
    temp.preRender(r);
    mElements.push_back(temp);
}

void ley::UIMenu::pushUIElement(const std::function<void()> &toggle, const std::function<bool()> &focus, const std::function<void()> &enter) {

    UIElement temp(toggle, focus, enter);
    mElements.push_back(temp);
}

int ley::UIMenu::count() {
    return mElements.size();
}

ley::UIElement* ley::UIMenu::getCurrentElementPtr() {
    return &mElements[mCurrentIndex];
}

int ley::UIMenu::getElementId(std::string label) {
    
    //Iterate through the elements and find the ID that matches the label.
    int index = -1;
    for(int i = 0; i < mElements.size(); ++i) {
        if (mElements[i].getLabel() == label) {
            index = i;
        }
    }
    return index;
}
void ley::UIMenu::addRenderables(ley::RenderablesPtr r) {
    mRenderables = r;
}

void ley::UIMenu::renderBaseMenuItems(ley::Video* v) {
    
    //Display all the base menu elements
    for(int i = 0; i < mElements.size() && !mElements.empty(); ++i) {
        SDL_Rect source = mElements[i].getSource();
        SDL_Rect destination = mElements[i].getDestination();
        SDL_Texture* baseTexture = mElements[i].getBase();

        if(baseTexture) {
            SDL_RenderCopy(v->getRenderer(), baseTexture, &source, &destination);
        }
        else if(mElements[i].getBaseFontPtr()->getTexturePtr() == nullptr) {
            // assume this is the font type and pre render the font if its empty.
            mElements[i].preRender(v->getRenderer());
        }
    }
}

void ley::UIMenu::renderHotItem(ley::Video* v) {
    
    if(mElements.size() <= 0) {return; /*EARLY EXIT*/}

    if(mElements[mCurrentIndex].getToggleFunction()) {

        if(!mElements[mCurrentIndex].getInFocus()) {
            mElements[mCurrentIndex].getToggleFunction()();
        }
    }

    SDL_Rect src_rect = currentSrc();
    SDL_Rect dest_rect = currentDest();

    if(SDL_RectEmpty(&src_rect)) {
        return; //Assume we have one of the function pointer type elements.
    }
    
    SDL_Texture* mainTexture = mElements.at(mCurrentIndex).getTexture();
    if(!mainTexture) {
        //assume the font type element and pre render the font
        mElements[mCurrentIndex].getMainFontPtr()->preRender(v->getRenderer());
    }

    SDL_RenderCopy(v->getRenderer(), mainTexture, &src_rect, &dest_rect);

    SDL_Texture* hotTexture = mElements.at(mCurrentIndex).getTextureHot();
    if(!hotTexture) {
        //assume the font type element and pre render the font
        mElements[mCurrentIndex].getHotFontPtr()->preRender(v->getRenderer());
    }
    SDL_SetTextureAlphaMod(hotTexture, mFader.alpha());
    SDL_RenderCopy(v->getRenderer(), hotTexture, &src_rect, &dest_rect);

    mFader.runFrame(); // TODO this actually goes in update, but this works for now.
}

void ley::UIMenu::render(ley::Video* v) { 
    renderBaseMenuItems(v);
    renderHotItem(v);

    mRenderables.renderAll(v->getRenderer(), false);
}

/*
** accounts for gaps
*/
bool ley::UIMenu::validNext(ley::UIMenuItem inMenuItem) {

    next(inMenuItem);
    while(isCurrentCellNull() && mCurrentIndex != mElements.size()-1) {
        next(inMenuItem);
    }
}

void ley::UIMenu::runCommand(ley::Command command) {
    if(count() > 0) {
            
        if(command == ley::Command::UI_down || command == ley::Command::UI_right) {

            if(mWidth > 1 && command == ley::Command::UI_down) {
                next(ley::UIMenuItem::row);
            } else {
                next(ley::UIMenuItem::cell);
            }
        }
        
        if(command == ley::Command::UI_up || command == ley::Command::UI_left) {
            
            if(mWidth > 1 && (command == ley::Command::UI_up)) {
                previous(ley::UIMenuItem::row);
            } else {
                previous(ley::UIMenuItem::cell);
            }
        }
    }
}

bool ley::UIMenu::isCurrentCellNull() {
    return mElements.at(mCurrentIndex).getPlaceHolder();
}

int ley::UIMenu::row() {
    return rowAt(mCurrentIndex);
}

int ley::UIMenu::column() {
    return columnAt(mCurrentIndex);
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
    if(mCurrentIndex > 0) {

        //if we have an active ui element untoggle it before moving to the previous
        if(mElements[mCurrentIndex].getToggleFunction()) {
            if(mElements[mCurrentIndex].getInFocus()) {
                mElements[mCurrentIndex].getToggleFunction()();
            }
            //also run the enter function
            mElements[mCurrentIndex].getEnterFunction()();
        }

        if(item == UIMenuItem::cell) {

            if(mWidth > 1) {
                //SDL_Log("col: %i, row: %i", column(), row());
                //if we are at the first column in a row
                if( column() <= 0 ) {
                    
                    return;
                }
            }

            mCurrentIndex--;
        }
        else if (item == UIMenuItem::row) {
            
            int oldCurrentIndex = mCurrentIndex;
            
            mCurrentIndex -= mWidth;

            if(mCurrentIndex < 0) {
                mCurrentIndex = oldCurrentIndex;
            }
        }
    }
}

void ley::UIMenu::next(UIMenuItem item) {
    if(mCurrentIndex < mElements.size()-1) {

        //if we have an active ui element untoggle it before moving to the next
        if(mElements[mCurrentIndex].getToggleFunction()) {
            if(mElements[mCurrentIndex].getInFocus()) {
                mElements[mCurrentIndex].getToggleFunction()();
            }
            //also run the enter function
            mElements[mCurrentIndex].getEnterFunction()();
        }

        if(item == UIMenuItem::cell) {

            if(mWidth > 1) {
                //if we are at the last column in a row
                if( column() >= (mWidth - 1) ) {
                    
                    return;
                }
            }

            mCurrentIndex++;
        }
        else if (item == UIMenuItem::row) {
            
            int oldCurrentIndex = mCurrentIndex;
            
            mCurrentIndex += mWidth;

            if(mCurrentIndex > mElements.size() - 1) {
                mCurrentIndex = oldCurrentIndex;
            }
        }
    }
}

SDL_Rect ley::UIMenu::currentDest() {
    return mElements.at(mCurrentIndex).getDestination();
}

SDL_Rect ley::UIMenu::currentSrc() {
    return mElements.at(mCurrentIndex).getSource();
}

int ley::UIMenu::getIndex() {
    return mCurrentIndex;
}

void ley::UIMenu::clear() {
    mElements.clear();
    mCurrentIndex = 0;
}

ley::UIElement* ley::UIMenu::getElementPtr(std::string label) {

    return &mElements.at(getElementId(label));
}