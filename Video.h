/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: Video System for the game.
Date: Feb/14/2020
*/

#pragma once

#include <SDL2/SDL.h>

#include "./inc/GameModel.h"
#include "Renderables.h"
#include "./inc/Sprite.h"
#include "Textures.h"
#include "./inc/Font.h"
#include "./inc/RectContainer.h"
#include "./inc/TextEntry.h"

namespace ley {

auto const LINES_POS_Y_PX = 40;
auto const LVL_POS_Y_PX = 65;
auto const SCORE_POS_Y_PX = 90;

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
    Uint32 frame_start;
    ley::RenderablesPtr mRenderables;
    ley::RenderablesPtr mRenderablesTopLayer;
    ley::RenderablesPtr mDebugRenderables;
    ley::RectContainer firstRectContainer;
    ley::RectContainer debugRectContainer;

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
    ley::Sprite spriteBackground;
    ley::Sprite spriteBackgroundfadeout;
    void createWindow();
    void createRenderer();
    void init();
    
    void loadTextures();

    void setBackgroundTexture(); //Create and set sprite object for the background.
    void updateScores(); //Update the fonts for scores with new model data.

    ley::Font fontLines; //Create the renderable font object for lines
    ley::Font fontLvl; //Create the font for the level ouput
    ley::Font fontScore; //Create the font for the score output
    ley::Font fontDebugMode; //Debug sign
    ley::Font SDLCompiled;
    ley::Font SDLLinked;
    ley::Font mFontCombo;

    void renderGridLines();

public:
    Video(ley::GameModel*);
    ~Video();

    void frameStart(); //Record time when game loops starts for locking the frame rate.
    void frameDelay(); //Calls SDL_Delay to delay frame based on mili_adjust
/* Accessors */
    bool fullScreen() {return fs;};
    bool getReady() {return video_ready;};
    SDL_Renderer* getRenderer() {return renderer;};
    SDL_Window* getWindow() {return window;};
    void setFullScreen(bool);
    void setRenderBackground(bool inRenderBackGround);
    void resetBackgroundFader();

/* Functions */
    void renderTopLayer();
    void render(); //Render additional bits.
    void renderSprites();
    void present(); //Present the rendered items to the user.
    void clear(); //Clear the backbuffer.
    void addRenderable(bool layer, ley::Renderable * r); //layer == true then debug renderable.
    void resetClock();
    void videoResizeBoard();
    void increaseTransparency();
    void decreaseTransparency();
};

}
