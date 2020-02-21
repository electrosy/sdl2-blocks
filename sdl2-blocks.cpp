/*
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A fun game and Mini Game Framework.
Date: Feb/15/2020
*/
#include <stdio.h>
#include <iostream>

#include <SDL2/SDL.h>

#include "Video.h"
#include "Input.h"
#include "Sprite.h"
#include "Block.h"
#include "GameModel.h"
#include "Clock.h"
#include "Winlet.h"
#include "Renderables.h"
#include "GameController.h"
#include "SimpleShape.h"
#include "Timer.h"
#include "Textures.h"

auto const TARGET_FPS = 60; //provide at least this many frames per second.
auto const DELAY_MILI = 1.3f; //start delay for the game loop
auto const MILI_ADJ_MAX = 1000;

ley::Textures* ley::Textures::instance = nullptr;
typedef ley::Textures TextureManager;

int main() {

    ley::Video mainVideo;
    TextureManager::Instance()->setRenderer(mainVideo.getRenderer());
    TextureManager::Instance()->loadTexture("assets/BlockPiece.bmp");
    bool game_running = 1;

    
    ley::Input mainInput;
    ley::GameModel mainGameModel;
    ley::GameController mainGameController(mainVideo.getRenderer(),&mainGameModel);

    ley::Sprite mainSprite(mainVideo.getRenderer(), "assets/BlockPiece.bmp");
    mainSprite.setPos(10,10);
    ley::Renderables renderables;
    renderables.push_back(&mainSprite);

    std::vector<SDL_Rect> catFrames;
    SDL_Rect catFrame1;
    catFrame1.x = 0; catFrame1.y = 75;
    catFrame1.h = 100; catFrame1.w = 100;
    catFrames.push_back(catFrame1);

    SDL_Rect catFrame2;
    catFrame2.x = 132; catFrame2.y = 75;
    catFrame2.h = 100; catFrame2.w = 100;
    catFrames.push_back(catFrame2);

    SDL_Rect catFrame3;
    catFrame3.x = 261; catFrame3.y = 75;
    catFrame3.h = 100; catFrame3.w = 100;
    catFrames.push_back(catFrame3);

    SDL_Rect catFrame4; 
    catFrame4.x = 400; catFrame4.y = 75;
    catFrame4.h = 100; catFrame4.w = 100;
    catFrames.push_back(catFrame4);

    SDL_Rect catFrame5; 
    catFrame5.x = 8; catFrame5.y = 207;
    catFrame5.h = 100; catFrame5.w = 100;
    catFrames.push_back(catFrame5);

    SDL_Rect catFrame6; 
    catFrame6.x = 138; catFrame6.y = 207;
    catFrame6.h = 100; catFrame6.w = 100;
    catFrames.push_back(catFrame6);

    ley::Sprite catSprite(mainVideo.getRenderer(), "assets/char9.png", 100, &catFrames);
    catSprite.setPos(695,540);
    renderables.push_back(&catSprite);
    ley::Sprite catSprite2(mainVideo.getRenderer(), "assets/char9.png", 175, &catFrames);
    catSprite2.setPos(100,100);
    renderables.push_back(&catSprite2);
    
    // test Winlet
    SDL_Rect debugBounds;
    debugBounds.x = 200; debugBounds.y = 200;
    debugBounds.h = 300; debugBounds.h = 300;
    SDL_Color debugBoundsColor = {100,100,100,100};
    ley::Winlet debugWinlet(debugBounds,debugBoundsColor);

    mainGameModel.debugBoard();

    //Test SimpleShape
    std::vector<SDL_Rect> rects;
    ley::SimpleShape firstSimpleShape(mainVideo.getRenderer());
    renderables.push_back(&firstSimpleShape);
    firstSimpleShape.addShape("window-title",{10,10,100,10});
    firstSimpleShape.addShape("window",{10,20,100,100});
 
    //Test Timer
    ley::Timer firstTimer(mainVideo.getRenderer(),3000,{10,300,100,50}); // a 3 second timer
    renderables.push_back(&firstTimer);

    //Test Timer
    ley::Timer secondTimer(mainVideo.getRenderer(),2500,{10,400,100,25}); // a 2 second timer
    renderables.push_back(&secondTimer);

    //Test Timer
    ley::Timer thirdTimer(mainVideo.getRenderer(),1000,{10,425,100,30}); // a 2 second timer
    renderables.push_back(&thirdTimer);

    //Test Timer
    ley::Timer fourthTimer(mainVideo.getRenderer(),333,{10,455,100,5}); // a 2 second timer
    renderables.push_back(&fourthTimer);

    //Temp texture for test rendering the board.
    SDL_Surface* temp_surface = IMG_Load("assets/BlockPiece.bmp");
    SDL_Texture* temp_tex = SDL_CreateTextureFromSurface(mainVideo.getRenderer(), temp_surface);
    SDL_FreeSurface(temp_surface);

    /**** Main Game Loop ****/
    SDL_Log("Starting Game loop!");
    ley::Clock mainClock;
    size_t frame_count = 0;
    unsigned fpsAdjustMili = 0;
    bool fs = 0; //full screen
    while(game_running) {
        SDL_Delay(DELAY_MILI + fpsAdjustMili);
    /*** RENDER ***/
        /* Render Sprites to buffer */
        auto avgFPS = 0;
        renderables.renderAll(); // render all sprites
        mainGameController.renderBoard(temp_tex);
        /* Output report */
        if (SDL_GetTicks() % 1000 == 0 ) {
            auto seconds_from_start = mainClock.secondsFromStart();
            avgFPS = seconds_from_start == 0 
                ? seconds_from_start : frame_count/seconds_from_start;
            SDL_Log(("Seconds From Start:" + std::to_string(seconds_from_start)).c_str());
            SDL_Log(("AVG FPS: " + std::to_string(avgFPS)).c_str());
        }
        
        /* Render the Video system to the screen */
        mainVideo.render(); // output to the video system.

    /*** GET INPUT ***/
        //pollEvents updates running and full screen flags
        mainInput.pollEvents(game_running,fs,mainGameModel);
        mainVideo.setFullScreen(fs);

    /*** UPDATE ***/
        /* Calculate Frame Rate and output to log */
        //adjust for target fps.
        if(avgFPS != 0) {
            auto avg_target_ratio = TARGET_FPS / avgFPS; //greater than 1 too slow, less than one too fast.
            if(avg_target_ratio > 0.954445f) { /*then speed up*/  /* TODO The point at which to speed up may need to 
                                                            be veriable. To ensure we dont dip below, we 
                                                            should probably adjust right before hitting 1
                                                            This adjustment may be very system specific, 
                                                            may be best to save the adjustment off to a file*/
                
                fpsAdjustMili -= 5;
                if(fpsAdjustMili < 0) {fpsAdjustMili = 0;}
            } else if(avg_target_ratio < 0.7999f) { /* then slow down */ /* NOTE slowing down seems easier   
                                                                            because you dont notice when its too fast. */
                
                if(fpsAdjustMili < MILI_ADJ_MAX) {
                    ++fpsAdjustMili;
                }
            } else { fpsAdjustMili = 0; }
        }

        if (SDL_GetTicks() % 1000 == 0 ) { 
            SDL_Log(("FpsMiliAdjust:" + std::to_string(fpsAdjustMili)).c_str());
        }

        firstTimer.runFrame();
        secondTimer.runFrame();
        thirdTimer.runFrame();
        fourthTimer.runFrame();
        mainGameController.runFrame();
        ++frame_count;

    /*** CLEAR ****/
        mainVideo.clear();
    }

    return 1;
}