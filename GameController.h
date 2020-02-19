/* 
Author: Steven Philley
Purpose: Controlls the flow of the game and moves the active 
         block when its time to.
Date: Feb/15/2020
*/
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Clock.h"
#include "Block.h"

namespace ley {

class GameController {

private:
   Clock blockDownClock;
public:
    GameController();
    ~GameController();

    void goNext(); //setup the next round, a round is a new block after the last has been placed.
    void runFrame(); // call this for each frame in the game to control the game model.
};

}
#endif
