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

    SDL_Log("Open font");
    Classic = TTF_OpenFont("assets/fonts/MartianMono-Regular.ttf", 24);
    if(!Classic) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }

    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = w;
    Message_rect.h = h;

    updateMessage("");
}

ley::Font::~Font() {

    if(Message) {
        SDL_DestroyTexture(Message);
    }
    SDL_Log("Close font");
    TTF_CloseFont(Classic);
    TTF_Quit();
}
//copy assignment operator
ley::Font& ley::Font::operator=(ley::Font other) {
    
    Message = nullptr;

    Message_rect.x = other.Message_rect.x;
    Message_rect.y = other.Message_rect.y;
    Message_rect.w = other.Message_rect.w;
    Message_rect.h = other.Message_rect.h;

    updateMessage("");

    return *this;
}
ley::Font ley::Font::operator()(ley::Font& other) {

    Message = nullptr;

    SDL_Log("Open font");
    Classic = TTF_OpenFont("assets/fonts/MartianMono-Regular.ttf", 24);
    if(!Classic) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }

    Message_rect.x = other.Message_rect.x;  //controls the rect's x coordinate
    Message_rect.y = other.Message_rect.y; // controls the rect's y coordinte
    Message_rect.w = other.Message_rect.w; // controls the width of the rect
    Message_rect.h = other.Message_rect.h; // controls the height of the rect

    updateMessage("");

    return *this;
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

std::string* ley::Font::getMessagePtr() {
    return &textMessage;
}

void ley::Font::render(SDL_Renderer * r, bool d) {

    SDL_Surface* surfaceMessage;
    surfaceMessage = TTF_RenderText_Solid(Classic, textMessage.c_str(), White);
    Message = SDL_CreateTextureFromSurface(r, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    
    int w;
    int h;
    SDL_QueryTexture(Message,
                     NULL, NULL,
                     &w, &h);
    Message_rect.h = h;
    Message_rect.w = w;

    SDL_RenderCopy(r, Message, NULL, &Message_rect);
    SDL_DestroyTexture(Message);
}

TTF_Font* ley::Font::getTTFFont() {
    return Classic;
}

void ley::Font::setPos(SDL_Point p) {
    Message_rect.x = p.x;
    Message_rect.y = p.y;
}

std::pair<int, int> ley::Font::size() {
    int width = -1;
    int height = -1;

    TTF_SizeText(Classic, textMessage.c_str(), &width, &height);

    return std::make_pair(width, height);
}