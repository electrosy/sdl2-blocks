/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A GameModel stores a representation of the game state.
Date: Feb/15/2020
*/
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <array>

#include "Block.h"

namespace ley {

const auto BOARDSIZE_WIDTH = 10;
const auto BOARDSIZE_HEIGHT = 20;
const auto BLOCKSIZE_MAX_HW = 4; //the largest possible height and width(square) we will need for a single block.
const auto BLOCK_PUT_X = 4; const auto BLOCK_PUT_Y = 0;
const auto MAX_BLOCKS_ON_BOARD = 500; //the max number of possible blocks that will be on the board.
enum class Direction {down,right,up,left};

class GameModel {

private:
   std::array<std::array<BlockTexCode, BOARDSIZE_WIDTH>, BOARDSIZE_HEIGHT > board;
   Block activeBlock;
   Block oldBlock;
   void clearBoard();
   void clearOldBlock();
   void putBlock(Block&); 
   bool canPut(Block&, Direction d);
public:
    GameModel();
    ~GameModel();

    std::array<std::array<BlockTexCode, BOARDSIZE_WIDTH>, BOARDSIZE_HEIGHT >*
    getBoard();

    void downExpired(); //block automaticly moves down based on a time interval
    void moveBlock(Direction d);
    void debugBoard(); //print the board to the console
    //std::string getBoard
};

}
#endif
