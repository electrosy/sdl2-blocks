/* 
Author: Steven Philley
Purpose: Representation of a sprite object on the screen
Date: Feb/17/2020
*/
#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace ley {

class Sprite {

private:
    SDL_Texture* texture;
    SDL_Rect dest_rect; 
    SDL_Renderer* renderer; //the renderer that we output the sprite to.
    bool multiframe = 0;
    std::vector<SDL_Rect> frames;
  
public:
    Sprite(SDL_Renderer*, const char*, bool = 0, std::vector<SDL_Rect>* = nullptr);
    ~Sprite();

    void render(int, int, int = 0);
    void clear();
};

}
#endif
