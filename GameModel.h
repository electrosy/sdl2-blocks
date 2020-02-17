/* 
Author: Steven Philley
Purpose: A GameModel stores a representation of the game state.
Date: Feb/15/2020
*/
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>

#include "Sprite.h"

namespace ley {

class GameModel {

private:
    char Array[10][20]; //the Game board
public:
    GameModel();
    ~GameModel();
};

}
#endif
