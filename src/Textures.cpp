/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/20/2020
*/
#include "../inc/Textures.h"

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
    
    if(textures.find(s) == textures.end()) {
        SDL_Log("Can not find texture! Aborting!");
        return nullptr;
    }

    return textures.find(s)->second;
}

void ley::Textures::loadTexture(const char* p, std::string texturename) {
    
    SDL_Log("Loading texture, path: %s key: %s", p, texturename.c_str());
    SDL_Surface* temp_surface = IMG_Load(p);
    if(!temp_surface) {
        SDL_Log("IMG_Load failed: %s", IMG_GetError());
    }
    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, temp_surface);
    if(t == NULL) {
        SDL_Log("Possible Error_loadTexture(): %s", SDL_GetError());
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
