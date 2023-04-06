/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Jul/14/2020
*/

#include "Font.h"

ley::Font::Font(int x, int y, int w, int h)
:
Message(nullptr) {

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

    updateMessage("");
}
ley::Font::~Font() {

    SDL_DestroyTexture(Message);
    TTF_Quit();
}

void ley::Font::updateMessage(std::string s) {
    //Only update the texture if the message has changed.
    if(s != textMessage) {
        textMessage = (s);
    }
}

std::string ley::Font::getMessage() {
    return textMessage;
}

void ley::Font::render(SDL_Renderer * r) {

    if(Message != nullptr) {
        SDL_DestroyTexture(Message);
        Message = nullptr;
    }

    SDL_Surface* surfaceMessage;
    surfaceMessage = TTF_RenderText_Solid(Classic, textMessage.c_str(), White); 
    Message = SDL_CreateTextureFromSurface(r, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);

    SDL_RenderCopy(r, Message, NULL, &Message_rect);
}