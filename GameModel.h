/* 
Author: Steven Philley
Purpose: A GameModel stores a representation of the game state.
Date: Feb/15/2020
*/
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <array>

#include "Block.h"

const auto BOARDSIZE_WIDTH = 10;
const auto BOARDSIZE_HEIGHT = 20;
const auto BLOCKSIZE_MAX_HW = 4; //the largest possible height and width(square) we will need for a single block.
const auto MAX_BLOCKS_ON_BOARD = 500;

namespace ley {

class GameModel {

private:
   std::array< std::array<std::string, BOARDSIZE_WIDTH>, BOARDSIZE_HEIGHT > board;
   Block activeBlock;
   void clearBoard();
   void putActive();
public:
    GameModel();
    ~GameModel();

    void downExpired(); //block automaticly moves down based on a time interval
    void moveBlock();
    void isBottom(); //test if block is at bottom.

    void debugBoard(); //print the board to the console
};

}
#endif
