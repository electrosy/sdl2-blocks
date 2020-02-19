/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/18/2020
*/

#include <SDL2/SDL.h>

#include "GameController.h"

/* RAII */
ley::GameController::GameController() {
    
}

ley::GameController::~GameController() {

}
/* Functions */
void ley::GameController::goNext() {

   // activeBlock = Block(0,4);
    blockDownClock.reset();
}

void ley::GameController::runFrame() {

}
