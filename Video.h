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
#include "Renderables.h"
#include "Sprite.h"
#include "Textures.h"

namespace ley {

class Video {

private:
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool video_ready; //video is woken up and initialized.
    bool sdl_fullscreen;
    ley::GameModel* gm;
    double mili_adjust; //number of mili seconds to delay the frame
    ley::Renderables renders;

    
    
    SDL_Rect cat_frame1 = {0,75,100,100};
    SDL_Rect cat_frame2 = {132,75,100,100};
    SDL_Rect cat_frame3 = {261,75,100,100};
    SDL_Rect cat_frame4 = {400,75,100,100};
    SDL_Rect cat_frame5 = {8,207,100,100};
    SDL_Rect cat_frame6 = {138,207,100,100};
    std::vector<SDL_Rect>cat_frames = {cat_frame1, cat_frame2,
        cat_frame3, cat_frame4, cat_frame5, cat_frame6}; //animation frames

    //sprites
    ley::Sprite catSprite;
    ley::Sprite catSprite2;
    
    void createWindow();
    void createRenderer();
    void init();
    void loadTextures();
    void loadSprites();

    void renderBackground(); //Render the background image.

public:
    Video(ley::GameModel*);
    ~Video();

    void frameDelay(); //Calls SDL_Delay to delay frame based on mili_adjust
/* Accessors */
    bool getReady() {return video_ready;};
    SDL_Renderer* getRenderer() {return renderer;};
    SDL_Window* getWindow() {return window;};
    void setFullScreen(bool);

/* Functions */
    void render(); //Render additional bits.
    void renderSprites();
    void present(); //Present the rendered items to the user.
    void clear(); //Clear the backbuffer.
};

}
#endif
