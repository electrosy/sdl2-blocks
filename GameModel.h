/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A GameModel stores a representation of the game state.
Date: Feb/15/2020
*/
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <array>

#include "Audio.h"
#include "Block.h"
#include "Clock.h"
#include "Timer.h"
#include "Input.h"
#include "HighScores.h"

namespace ley {

enum class StateChange { //TODO this can probably be called State
    play,
    options,
    quitstate,
    highscores,
    gameover,
    credits,
    none}; //particular states that the game can be in

enum class UIFocusChange {
    goto_textBox,
    textBox,
    goto_game,
    game
};


const auto BOARDSIZE_WIDTH = 10; // TODO ADD FEATURE - this should be the first option added.
const auto BOARDSIZE_BUFFER = 3; // buffer above board for piece to come into play
const auto BOARDSIZE_HEIGHT = 20 + BOARDSIZE_BUFFER; //add 3 additional blocks so tetrominos can enter play in the right place.
const auto BLOCKSIZE_PX = 30; // blocksize in pixels
const auto BOARD_POS_X_PX = 490; //Board starting position x for block, when drawing border subtract 1px.
const auto BOARD_POS_Y_PX = 40; //Board starting position y for block, when drawing border subtract 1px.
const auto BLOCK_START_POS_Y_PX = BOARD_POS_Y_PX - (BLOCKSIZE_PX*BOARDSIZE_BUFFER); //Subtract three rows from board start_y //Number of pixels to offset the board.
const auto BLOCK_START_POS_X_PX = BOARD_POS_X_PX;
const auto NEXTBOX_PADDING_PX = 15; // padding from the game board
const auto NEXTBOX_SIZE_PX = (BLOCKSIZE_PX * 4) + 6; // four blocks can fit in the next border plus 5px padding.
const auto NEXTBOX_POS_X_PX = BOARD_POS_X_PX - NEXTBOX_PADDING_PX - NEXTBOX_SIZE_PX;
const auto NEXTBOX_POS_Y_PX = BOARD_POS_Y_PX;
const auto NEXTBLOCK_OFFSET_X_PX = BLOCK_START_POS_X_PX - 130;
const auto NEXTBLOCK_OFFSET_Y_PX = 10;
const auto NEW_LVL_AT_LINES = 10;


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
    bool newLevelToReport{true}; //true if the level has changed but has not been passed along yet, start with true for game init.
    long score; //the total score the this game (level*linesatonce)
    bool gameRunning;
    bool active; //not paused // TODO this should be called paused and the checks should be reversed
    void clearOldBlock();
    void putBlock(Block&);
    bool canPut(Block&, Command d); //can physically put the block in the board.
    std::vector<char> checkForLines(char start);
    int firstLineAt(int); //returns the first complete line from the bottom or -1 if there is no line.
    bool processLines(); //returns true if any number of lines are removed.
    void clearAndRecordLine(/*int, int*/ int lineNum); //clear the completed lines and keep track of the score.
    void shiftBoard(char, char); //start line, number of lines
    void fillTop(char); //fill top of the board after the shift
    ley::Block getRandomBlock();
    ley::Block debugGetBlock();
    void updateSpeed(); //check to see if the speed of the falldown block needs to change based on lines/score
    bool running; //if true then the program is still runing and has not been asked to exit yet.
    int calcLevel(); //Calculate current level based on number of lines completed
    bool debugCommandsFlag = false; //allow debug commands
    bool mDebugOnlyLine = false; //use only the line block for testing purposes
    ley::Audio audSystem; //audio subsystem.
    ley::StateChange mStateChange = ley::StateChange::none;

    ley::HighScores mHighScores;
    bool mNewHighScore = false;

    ley::UIFocusChange mUIFocusChange;

public:
    GameModel();
    ~GameModel();

    std::array<std::array<std::pair<BlockTexCode,bool>, BOARDSIZE_WIDTH>, BOARDSIZE_HEIGHT >*
    getBoard();
    void clearBoard();
    void debugFill();
    void debugResetActiveBlock();
    bool moveBlock(Command); //returns true for false if block actually moved
    bool rotateBlock(bool);
    bool canRotate(bool); //false for counterclockwise and true for clockwise
    void debugBoard(bool); //print the board to the console
    void overlayToggle();
    bool isOverlayOn();
//    bool canMoveDown(); //Can move down based on Game rules. // TODO no longer used.
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
    double speed() { return currentSpeed; };
    
    void pauseGame(bool);
    bool isPaused();
    bool programRunning(); //is the program running?
    void stopProgram(bool); //sets the program to exit by setting running to false.
    bool newLevel(); //retruns true if there is a new level to report.

    ley::Audio* audio() { return &audSystem; };

    ley::StateChange currentStateChange() { return mStateChange; }; //TODO this can probably be called gotoState
    void stateChange(ley::StateChange state) { mStateChange = state; };

    ley::HighScores* highScores() { return &mHighScores; };

    bool debugCommands(); //get current debugCommands flag
    void debugCommandsToggle();
    bool debugOnlyLine();
    void debugOnlyLineToggle();
    void debugNextLevel() {                   numLevel++; newLevelToReport=true; };
    void debugPrevLevel() { if(numLevel > 0){ numLevel--; } newLevelToReport=true; };

    void newHighScore(bool flag);
    bool newHighScore();

    UIFocusChange UIInputFocus();
    void UIInputFocus(ley::UIFocusChange fc);
    
};

}
#endif
