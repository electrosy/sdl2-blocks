/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: Video System for the game.
Date: Feb/14/2020
*/
#ifndef VIDEO_H
#define VIDEO_H

#include <SDL2/SDL.h>

#include "GameModel.h"

namespace ley {

class Video {

private:
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool video_ready; //video is woken up and initialized.
    bool sdl_fullscreen;
    ley::GameModel* gm;
    
    void createWindow();
    void createRenderer();
    void init();
    void loadTextures();

    void renderBackground(); //Render the background image.

public:
    Video(ley::GameModel*);
    ~Video();

/* Accessors */
    bool getReady() {return video_ready;};
    SDL_Renderer* getRenderer() {return renderer;};
    SDL_Window* getWindow() {return window;};
    void setFullScreen(bool);

/* Functions */
    void render(); //Render additional bits.
    void present(); //Present the rendered items to the user.
    void clear(); //Clear the backbuffer.
};

}
#endif
