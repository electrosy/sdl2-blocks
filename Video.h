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
#include "Font.h"

auto const LINES_POS_X_PX = 800; //Score text
auto const LINES_POS_Y_PX = 40; 

auto const LVL_POS_X_PX = 800; //Level text
auto const LVL_POS_Y_PX = 60;

auto const SCORE_POS_X_PX = 800;
auto const SCORE_POS_Y_PX = 80;

namespace ley {

const auto START_X_OFFSET_PX = 490;

class Video {

private:
    

    SDL_Window* window;
    SDL_Renderer* renderer;
    Uint32 avg_fps;
    ley::Clock mainClock;
    size_t frame_count; //number of frames that the game has been running
    bool fs = false; //full screen
    bool video_ready; //video is woken up and initialized.
    ley::GameModel* gm;
    bool renderbg; //render background or not.
    double mili_adjust; //number of mili seconds to delay the frame
    ley::Renderables mRenderables;
    ley::Renderables mDebugRenderables;
    ley::SimpleShape firstSimpleShape;

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
    
    Uint32 avgFPS();
    void createWindow();
    void createRenderer();

    void init();
    void loadTextures();
    void loadSprites();

    void renderBackground(); //Render the background image.
    void updateScores(); //Update the fonts for scores with new model data.

    //Create the renderable font object for lines
    ley::Font fontOne;
    ley::Font* ptrFont;
    //Create the font for the level ouput
    ley::Font fontLvl;
    ley::Font* ptrFontLvl;
    //Create the font for the score output
    ley::Font fontScore;
    ley::Font* ptrFontScore;

    //Game over font
    ley::Font fontGameOver;
    
public:
    Video(ley::GameModel*);
    ~Video();

    void frameDelay(); //Calls SDL_Delay to delay frame based on mili_adjust
/* Accessors */
    bool fullScreen() {return fs;};
    void fullScreen(bool fs);
    bool getReady() {return video_ready;};
    SDL_Renderer* getRenderer() {return renderer;};
    SDL_Window* getWindow() {return window;};
    void setFullScreen(bool);
    void setRenderBackground(bool inRenderBackGround);

/* Functions */
    void render(); //Render additional bits.
    void renderSprites();
    void present(); //Present the rendered items to the user.
    void clear(); //Clear the backbuffer.
    void addRenderable(bool layer, ley::Renderable * r); //layer == true then debug renderable.

    void resetClock();
};

}
#endif
