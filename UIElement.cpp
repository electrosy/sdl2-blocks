/* 
sdl2-blocks
Copyright (C) 2021 Steven Philley

Purpose: see header.
Date: Jul/17/2021
*/
#include "UIElement.h"

ley::UIElement::UIElement(const std::function<void()> &toggle, const std::function<bool()> &focus, const std::function<void()> &enter) {
    mUIToggleFunc = toggle;
    mUIInFocus = focus;
    mCommitUI = enter;
}

ley::UIElement::UIElement(std::string l, SDL_Rect sr, SDL_Rect dr, SDL_Texture* b, SDL_Texture* t, SDL_Texture* th)
: label(l), source(sr), destination(dr), textureBase(b), texture(t), textureHot(th), hot(false) {

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
    return textureBase;
}

SDL_Texture* ley::UIElement::getTexture() {
    return texture;
}

SDL_Texture* ley::UIElement::getTextureHot() {
    return textureHot;
}

