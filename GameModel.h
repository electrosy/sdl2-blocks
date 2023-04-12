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
#include "Clock.h"
#include "Timer.h"

 
namespace ley {

const auto BOARDSIZE_WIDTH = 10; //ADD FEATURE - this should be the first option added, check width of 4 bug case.
const auto BOARDSIZE_HEIGHT = 23; //add 3 additional blocks so tetrominos can enter play in the right place.

const auto BOARD_OFFSET_PX = -50; //Number of pixels to offset the board.

const auto NEW_LVL_AT_LINES = 10;
//TODO rename up to rotate
enum class Direction {down,right,up,left,none,pause,space}; //these include directions and other inputs, this is overloaded maybe a couple enums would work better?

class GameModel {

private:
    std::array<std::array<std::pair<BlockTexCode,bool>, BOARDSIZE_WIDTH>, BOARDSIZE_HEIGHT> board;
    Block activeBlock;
    Block oldBlock;
    Block nextBlock;
    bool overlayOn;
    double currentSpeed; //how many miliseconds until the block falls down.
    int numLines; //number of lines the player has successfully completed. (Lines)
    int numLevel; //the current level we are on. A combination of lines. Aprox. 10 lines per level. (Level)
    long score; //the total score the this game (level*linesatonce)
    bool gameRunning;
    bool active; //not paused
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
    void updateSpeed(); //check to see if the speed of the falldown block needs to change based on lines/score
    bool running; //if true then the program is still runing and has not been asked to exit yet.

public:
    GameModel();
    ~GameModel();

    std::array<std::array<std::pair<BlockTexCode,bool>, BOARDSIZE_WIDTH>, BOARDSIZE_HEIGHT >*
    getBoard();

    double moveBlock(Direction); //will return a new falldown speed.
    void rotateBlock(bool);
    bool canRotate(bool); //false for counterclockwise and true for clockwise
    void debugBoard(bool); //print the board to the console
    void overlayToggle();
    bool isOverlayOn();
    bool canMoveDown(); //Can move down based on Game rules.
    bool newBlock();
    void setBlock();
    int getScore();
    int getLevel();
    int getLines();
    void addToScore(long);
    bool isGameRunning();
    void setGameRunning(bool running);
    ley::Block getNextBlock();
    std::string getRandomTexture();
    void resetGame();
    
    void pauseGame(bool);
    bool isPaused();
    bool programRunning(); //is the program running?
    void stopProgram(bool); //sets the program to exit.
};

}
#endif
