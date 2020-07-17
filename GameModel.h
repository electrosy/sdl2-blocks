/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A GameModel stores a representation of the game state.
Date: Feb/15/2020
*/
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <array>

#include "Block.h"

namespace ley {

const auto BOARDSIZE_WIDTH = 10;
const auto BOARDSIZE_HEIGHT = 20;
const auto BLOCKSIZE_MAX_HW = 4; //the largest possible height and width(square) we will need for a single block.
const auto BLOCK_PUT_X = 4; const auto BLOCK_PUT_Y = 0;
const auto MAX_BLOCKS_ON_BOARD = 500; //the max number of possible blocks that will be on the board.
enum class Direction {down,right,up,left,none};

class GameModel {

private:
    std::array<std::array<std::pair<BlockTexCode,bool>, BOARDSIZE_WIDTH>, BOARDSIZE_HEIGHT> board;
    Block activeBlock;
    Block oldBlock;
    double numLines; //number of lines the player has successfully completed. (score)
    void clearBoard();
    void clearOldBlock();
    void putBlock(Block&);
    bool canPut(Block&, Direction d); //can physically put the block in the board.
    unsigned int debug_phase = 0; //used to keep track of different phases of play
    std::pair<char,char> checkForLines(char); //check to see if any solid lines across the board have been made, return -1 for none
    int firstLineAt(int); //returns the first complete line from the bottom or -1 if there is no line.
    void processLines();
    void clearAndRecordLines(int, int); //clear the completed lines and keep track of the score.
    void shiftBoard(char, char); //start line, number of lines
    void fillTop(char); //fill top of the board after the shift

public:
    GameModel();
    ~GameModel();

    std::array<std::array<std::pair<BlockTexCode,bool>, BOARDSIZE_WIDTH>, BOARDSIZE_HEIGHT >*
    getBoard();

    void downExpired(); //block automaticly moves down based on a time interval
    void moveBlock(Direction);
    void rotateBlock(bool);
    bool canRotate(bool); //false for counterclockwise and true for clockwise
    void debugBoard(bool); //print the board to the console
    bool canMoveDown(); //Can move down based on Game rules.
    void newBlock();
    void setBlock();
    double getScore();
};

}
#endif
