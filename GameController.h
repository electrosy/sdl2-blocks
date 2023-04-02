/* 
sdl2-blocks
Copyright (C) 2020-2021 Steven Philley

Purpose: Controlls the flow of the game and moves the active 
         block when its time to.
Date: Feb/15/2020
*/
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Video.h"
#include "GameModel.h"
#include "Clock.h"
#include "Block.h"
#include "Font.h"
#include "Timer.h"

namespace ley {

class GameController {

private:
    ley::Video* v;
    SDL_Renderer* ren;
    ley::GameModel* gm;

    //Textures
    SDL_Texture *blockBits2 = nullptr;

    //Debug timers
    ley::Timer firstTimer;
    ley::Timer secondTimer;
    ley::Timer thirdTimer;
    ley::Timer fourthTimer;

    //Timers
    ley::Timer fallTimer; //Time to force the blockdown    

public:
    GameController(ley::Video* , ley::GameModel*);
    ~GameController();

    void runFrame(bool autoRestart, double newTime); // TODO temporary pass through params for the timers.
    ley::Timer* getFallTimer(); //TODO temporary accessor for the timer
    
    void renderBoard(); //renders the board to the video.


};

}
#endif
