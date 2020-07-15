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

namespace ley {

class GameController {

private:
    SDL_Renderer* ren;
    ley::GameModel* gm;
    void goNext(); //setup the next round, a round is a new block after the last has been placed.
public:
    GameController(SDL_Renderer* , ley::GameModel*);
    ~GameController();

    void runFrame(ley::Font*); // call this for each frame in the game to control the game model.
    
    void renderBoard(SDL_Texture*); //renders the board to the video.
};

}
#endif
