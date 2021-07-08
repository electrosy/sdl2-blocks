/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: Controlls the flow of the game and moves the active 
         block when its time to.
Date: Feb/15/2020
*/
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Video.h"
#include "Clock.h"
#include "Block.h"
#include "GameModel.h"
#include "Font.h"

#include "Timer.h"

namespace ley {

class GameController {

private:
    SDL_Renderer* ren;
    ley::GameModel* gm;
public:
    GameController(SDL_Renderer* , ley::GameModel*);
    ~GameController();

    void runFrame(ley::Font*, ley::Font*); // call this for each frame in the game to control the game model.
                                            //Fonts for, lines / score
    
    void renderBoard(); //renders the board to the video.
    void renderBackground(); //render the background to the screen.
};

}
#endif
