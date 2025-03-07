/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/20/2020
*/
#include "Textures.h"

/* RAII */
ley::Textures::Textures() {

}

ley::Textures::~Textures() {
    unloadTextures();
    
    delete instance;
    instance = nullptr;
}

void ley::Textures::unloadTextures() {
    //iterate through the map of textures and unload each one.
    for (const auto& [key, value] : textures) {
        SDL_DestroyTexture(value);
    }


}

/* Accessors */
void ley::Textures::setRenderer(SDL_Renderer* r) {
    renderer = r;
}

SDL_Texture* ley::Textures::getTexture(std::string s) {
    SDL_Log("ley::Textures::getTexture() key: %s", s.c_str());
    return textures.find(s)->second;
}

void ley::Textures::loadTexture(const char* p, std::string texturename) {
    
    SDL_Surface* temp_surface = IMG_Load(p);
    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, temp_surface);
    if(t == NULL) {
        SDL_Log("Possible Error_loadTexture():");
        SDL_Log("%s",SDL_GetError());
    }
    else {
        textures.insert(std::make_pair(texturename,t));
        ++count;
    }
    SDL_FreeSurface(temp_surface);
    
}
/* Functions */

/* Singleton */
ley::Textures* ley::Textures::Instance() {
    if(instance == nullptr) {
        instance = new ley::Textures();
        return instance;
    }

    return instance;
}
