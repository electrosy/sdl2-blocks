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
#include "Timer.h"
 
namespace ley {

const auto BOARDSIZE_WIDTH = 10;
const auto BOARDSIZE_HEIGHT = 23; //add 3 additional blocks so tetriminos can entry play in the right place.
const auto MAX_BLOCKS_ON_BOARD = 500; //the max number of possible blocks that will be on the board.
const auto BOARD_OFFSET_PX = -50; //Number of pixels to offset the board.
const auto START_X_OFFSET_PX = 362;
const auto NEW_LVL_AT_LINES = 10;
enum class Direction {down,right,up,left,none};

class GameModel {

private:
    std::array<std::array<std::pair<BlockTexCode,bool>, BOARDSIZE_WIDTH>, BOARDSIZE_HEIGHT> board;
    Block activeBlock;
    Block oldBlock;
    Block nextBlock;
    double currentSpeed; //how many miliseconds until the block falls down.
    int numLines; //number of lines the player has successfully completed. (Lines)
    int numLevel; //the current level we are on. A combination of lines. Aprox. 10 lines per level. (Level)
    bool gameOver;
    void clearBoard();
    void clearOldBlock();
    void putBlock(Block&);
    bool canPut(Block&, Direction d); //can physically put the block in the board.
    std::pair<char,char> checkForLines(char); //check to see if any solid lines across the board have been made, return -1 for none
    int firstLineAt(int); //returns the first complete line from the bottom or -1 if there is no line.
    void processLines();
    void clearAndRecordLines(int, int); //clear the completed lines and keep track of the score.
    void shiftBoard(char, char); //start line, number of lines
    void fillTop(char); //fill top of the board after the shift
    ley::Block getRandomBlock();
    ley::Timer* ptrTimer; //pointer to the timer so that the speed can be changed.
    void updateSpeed(); //check to see if the speed of the falldown block needs to change based on lines/score

public:
    GameModel(ley::Timer*);
    ~GameModel();

    std::array<std::array<std::pair<BlockTexCode,bool>, BOARDSIZE_WIDTH>, BOARDSIZE_HEIGHT >*
    getBoard();

    void downExpired(); //block automaticly moves down based on a time interval
    double moveBlock(Direction); //will return a new falldown speed.
    void rotateBlock(bool);
    bool canRotate(bool); //false for counterclockwise and true for clockwise
    void debugBoard(bool); //print the board to the console
    bool canMoveDown(); //Can move down based on Game rules.
    bool newBlock();
    void setBlock();
    int getScore();
    int getLevel();
    bool isGameOver();
    ley::Block getNextBlock();
    std::string getRandomTexture();
};

}
#endif
