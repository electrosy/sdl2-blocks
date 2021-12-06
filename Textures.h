/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A container for loading textures (single graphics files) from file and retrieving their SDL_Texture type.
         See Texture for animated frames.
         A singleton
Date: Feb/20/2020
*/
#ifndef TEXTURES_H
#define TEXTURES_H

#include <map>
#include <string>
#include <array>

#include <SDL2/SDL_image.h>

namespace ley {

class Textures {

private:
    std::map<std::string, SDL_Texture*> textures;
    SDL_Renderer* renderer;
    unsigned int count; //number of textures we have loaded in the system.
    Textures();
    static Textures* instance;
    void unloadTextures();
public:
    static Textures* Instance();
    ~Textures();
    /* Accessors */
    void loadTexture(const char*, std::string);
    SDL_Texture* getTexture(std::string);
    void setRenderer(SDL_Renderer*);
};

}
#endif
