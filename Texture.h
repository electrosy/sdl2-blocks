/* 
Author: Steven Philley
Purpose: Wrap SDL Texture which is a Renderable.
Date: Feb/17/2020
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace ley {

class Texture {

private:
    SDL_Texture* texture;
  
public:
    Texture();
    ~Texture();
};

}
#endif
