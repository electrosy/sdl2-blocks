/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley

Purpose: see header.
Date: Jul/17/2021
*/
#include "../../inc/UI/UIElement.h"

ley::UIElement::UIElement(const std::function<void()> &toggle, const std::function<bool()> &focus, const std::function<void()> &enter) {
    mUIToggleFunc = toggle;
    mUIInFocus = focus;
    mCommitUI = enter;
    source = {-1,-1,-1,-1};
    destination = {-1,-1,-1,-1};
}

ley::UIElement::UIElement(std::string l, SDL_Rect sr, SDL_Rect dr, std::string message, int size)
: label(l), source(sr), destination(dr), hot(false), mMessage(message) {

    mBaseFont = mHotFont = mMainFont = {destination.x, destination.y, destination.w, destination.h};
    mBaseFont.updateMessage(mMessage);
    mHotFont.updateMessage(mMessage);
    mMainFont.updateMessage(mMessage);

    mBaseFont.setColor(CDARKTEAL);
    mHotFont.setColor(CBRIGHTRED);
    mMainFont.setColor(CWHITE);

    mBaseFont.setFontSize(size);
    mHotFont.setFontSize(size);
    mMainFont.setFontSize(size);
}

ley::UIElement::~UIElement() {

}

void ley::UIElement::setActiveSelector(bool h) {
    hot = h;
}

bool ley::UIElement::isActiveSelector() {
    return hot;
}

std::string ley::UIElement::getLabel() {
    return label;
}

SDL_Rect ley::UIElement::getSource() {
    return source;
}

SDL_Rect ley::UIElement::getDestination() {
    return destination;
}

SDL_Texture* ley::UIElement::getBase() {
    
    return mBaseFont.getTexturePtr();
}

SDL_Texture* ley::UIElement::getTexture() {
    
    return mMainFont.getTexturePtr();
}

SDL_Texture* ley::UIElement::getTextureHot() {
    
    return mHotFont.getTexturePtr();
}

void ley::UIElement::preRender(SDL_Renderer* r) {

    mMainFont.preRender(r);
    mHotFont.preRender(r);
    mBaseFont.preRender(r);

    setDimensions();

}

void ley::UIElement::setMessage(std::string message) {
    
    mMainFont.updateMessage(message);
    mBaseFont.updateMessage(message);
    mHotFont.updateMessage(message);
    
}

void ley::UIElement::setDimensions() {

    //Assume that all 3 fonts are the same size.
    int w;
    int h;
    SDL_QueryTexture(mBaseFont.getTexturePtr(),
                     NULL, NULL,
                     &w, &h);
    
    source.x = 0;
    source.y = 0;
    source.w = w;
    source.h = h;

    destination.w = w;
    destination.h = h;
}

void ley::UIElement::setFontSize(int size) {

    mBaseFont.setFontSize(size);
    mHotFont.setFontSize(size);
    mMainFont.setFontSize(size);
}

