/* 
Author: Steven Philley
Purpose: Wrap SDL Texture which is a Renderable. Renderables
         are renderable with SDL_RenderCopy
Date: Feb/17/2020
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Renderable.h"

namespace ley {

class Texture : public Renderable {

private:

protected:
    SDL_Texture* texture;
    std::vector<SDL_Rect> frames;
    SDL_Rect dest_rect;
  
public:
    Texture(SDL_Renderer*, const char*, std::vector<SDL_Rect>*);
    ~Texture();

   void render(int, int, int = 0);
};

}
#endif
