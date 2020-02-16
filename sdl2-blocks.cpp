/* 
Author: Steven Philley
Purpose: A fun game.
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

int main() {
    bool game_running = 1;

    ley::Video mainVideo;
    ley::Input mainInput;

    ley::GameModel mainGameModel;

    ley::Sprite mainSprite(mainVideo.getRenderer(), "assets/BlockPiece.bmp");

    // TODO create a vector of SDL_Rect to define where the frames are in the sheet.
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
    
    ley::Sprite catSprite(mainVideo.getRenderer(), "assets/char9.png",1, &catFrames);
    ley::Sprite catSprite2(mainVideo.getRenderer(), "assets/char9.png",1, &catFrames);

    // TODO Create a single sprite block
    ley::Block firstBlock(mainSprite); //test block, not animated
    // TODO Create an animated test block
    ley::Block catAnimated(catSprite);

    // TODO Create a constructed test block

    /**** Main Game Loop ****/
    SDL_Log("Starting Game loop!");
    while(game_running) {
        SDL_Delay(1);

    /*** RENDER ***/
        /* Render Sprites to buffer */
        auto current_frame = int(((SDL_GetTicks() / 175) % 6));
        {
            catSprite.render(695,540,current_frame);
            catSprite2.render(100,100,current_frame);
            mainSprite.render(10,10);
           // firstBlock.getFrame()->render(20,20);
           // catAnimated.getFrame()->render(30,30);
        }
        /* Render the Video system to the screen */
        mainVideo.render();

    /*** GET INPUT ***/
        game_running = mainInput.pollEvents();

    /*** UPDATE ***/
        /* Calculate Frame Rate and output to log */
        // TODO create timer object
        /*
        auto avgFPS = current_frame / ( fpsTimer.getTicks() / 1000.f );
        if( avgFPS > 2000000 )
        {
            avgFPS = 0;
        }
        */

    /*** CLEAR ****/
        mainVideo.clear();
    }

    //printf("Exiting!");

    return 1;
}