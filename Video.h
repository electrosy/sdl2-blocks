/* 
Author: Steven Philley
Purpose: Video System for the game.
Date: Feb/14/2020
*/

#ifndef VIDEO_H
#define VIDEO_H

#include <SDL2/SDL.h>

namespace ley {

class Video {

private:
    void createWindow();
    void createRenderer();
    void init();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool sdl_ready; //system is woken up and initialized.
public:
    Video();
    ~Video();

/* Accessors */
    bool getReady() {return sdl_ready;}
    SDL_Renderer* getRenderer() {return renderer;};

/* Functions */
    void render();
    void clear();
};

}
#endif
