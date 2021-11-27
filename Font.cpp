/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Jul/14/2020
*/

#include "Font.h"

ley::Font::Font(SDL_Renderer* r, int x, int y, int w, int h)
: Renderable(r) {

    if (TTF_Init() < 0) {
        SDL_Log("TTF_Init failed");
    }

    Classic = TTF_OpenFont("assets/arcadeclassic.ttf", 16);
    if(!Classic) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }

    Message_rect.x = x;  //controls the rect's x coordinate
    Message_rect.y = y; // controls the rect's y coordinte
    Message_rect.w = w; // controls the width of the rect
    Message_rect.h = h; // controls the height of the rect

    updateMessage("Score");
    updateTexture();

}
ley::Font::~Font() {

    SDL_DestroyTexture(Message);
}
void ley::Font::updateTexture() {
    
    SDL_Surface* surfaceMessage;
    surfaceMessage = TTF_RenderText_Solid(Classic, textMessage.c_str(), White); 
    Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    
    SDL_FreeSurface(surfaceMessage);
}
void ley::Font::updateMessage(std::string s) {
    textMessage = (s);
    updateTexture();
}

std::string ley::Font::getMessage() {
    return textMessage;
}

void ley::Font::render() {
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
}