/* 
Author: Steven Philley
Purpose: Representation of a sprite texture
Date: Feb/14/2020
*/

#ifndef SPRITE_H
#define SPRITE_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace ley {

class Sprite {

private:
    SDL_Texture* texture;
    // the new SDL_Texture variable 
    SDL_Rect source_rect; 
    // the first rectangle 
    SDL_Rect dest_rect; 
    // another rectangle

    SDL_Renderer* renderer; //the renderer that we output the sprite to.
  
public:
    Sprite(SDL_Renderer*, const char*);
    ~Sprite();

    void render(int, int);
    void clear();
};

}
#endif
