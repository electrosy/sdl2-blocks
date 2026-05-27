/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: GameModel gameplay core — game loop, block movement, rotation,
         line clearing, scoring, and speed.
         Setup/config/binding methods live in GameModel_setup.cpp.
Date: Feb/15/2020
*/
#include <array>

#include <SDL2/SDL.h>
#include "../inc/GameModel.h"
#include "../inc/ConfigIO.h"
#include "../inc/Rand_int.h"


/* RAII */
ley::GameModel::GameModel()
:
mNumLines(0),
mStartLevel(1),
mNumLevel(mStartLevel),
mGameRunning(true), 
mCurrentSpeed(1000.0f), 
mPaused(false), 
mOverLayOn(false),
mScore(0),
mProgramRunning(true),
mDebugOnlyLine(false) {

    SDL_Log("ley::GameModel::ctor");
    readBlockData();
    ley::Block::setBlockDataPtr(&mBlockMapData);
    initGame();
    mHighScores.read();
    loadKeyBindings();
    loadButtonBindings();
    ConfigIO::readMainConfig(this);
    readConfigOther();
    mActiveBlock.setBlockDataPtr(&mBlockMapData);
    mLanguageModel.loadLanguage();
    mStartLevel = calcLevel();

    if(mStartLevel > mNumLevel) {
        mNumLevel = mStartLevel;
    }
}

ley::GameModel::~GameModel() {
    ConfigIO::writeMainConfig(this);
    ConfigIO::writeKeyboardBindings(mKeyBindings);
    ConfigIO::writeGamepadBindings(mButtonBindings);
}

/* Accessors */
ley::Block ley::GameModel::getNextBlock() {
    return mNextBlock;
}
bool ley::GameModel::programRunning() {
    return mProgramRunning;
}

void ley::GameModel::stopProgram(bool stop) {
    mProgramRunning = !stop;
}

bool ley::GameModel::isGameRunning() {
    return mGameRunning;
}

int ley::GameModel::getScore() {
    return mScore;
}

int ley::GameModel::getLines() {
    return mNumLines;
}

ley::Board* ley::GameModel::getBoard() {
    return &mBoard;
}

/* Functions */
void ley::GameModel::addToScore(long p) {
    mScore += p;
}

void ley::GameModel::debugResetActiveBlock() {
    
    mActiveBlock = debugGetBlock();
    mOldBlock = mActiveBlock;
    mOldBlock.setClear(true);
    mActiveBlock.debugResetPos();
    mOldBlock.debugResetPos();
}

void ley::GameModel::clearOldBlock() {
    //new board
    mBoard.putBlock(mOldBlock);
}
ley::Block ley::GameModel::debugGetBlock() {

    return Block(4,0,BlockNameType::tee,false);
}

ley::Block ley::GameModel::getRandomBlock() {

    static constexpr BlockNameType BLOCK_TYPES[] = {
        BlockNameType::tee,  BlockNameType::rLee, BlockNameType::zee,
        BlockNameType::mzee, BlockNameType::lLee, BlockNameType::line,
        BlockNameType::cube,
    };

    if (mDebugOnlyLine)
        return Block(mBoard.width(), 2, BlockNameType::line, false);

    Rand_int rand0to6(0, 6);
    Block a(mBoard.width(), 2, BLOCK_TYPES[rand0to6()], false);

    // Position so the bottom of the block appears at the top of the board
    a.getPosRectPtr()->y -= a.getPosRectPtr()->h;

    return a;
}

bool ley::GameModel::newBlock() {
    SDL_Log("New Block");

    mActiveBlock = mNextBlock;
    mOldBlock = mActiveBlock;
    mOldBlock.setClear(true);
    mNextBlock = getRandomBlock();

    //check if we can put down the new active block.
    if(!mBoard.canPut(mActiveBlock, ley::Command::up).first) {
        return false;
    }
    else {
        mBoard.putBlock(mActiveBlock);
        return true;
    }
   
    return false;
}

bool ley::GameModel::rotateWithKick(bool r) {

    int kick = 0; //amount of wall kick to the right or left.
    // TODO think about the case where one kick doesn't work, but possibly two kicks or more would.

    int boardWidth = mBoard.width();
    const int MAX_KICK = boardWidth/2 > BLOCK_SIZE ? BLOCK_SIZE : boardWidth/2;
    std::pair<bool, std::string> result = {false, ""};
    result = rotateBlock(r);

    // TODO we need to make this check more generic to support the block editor.
    // If the block can not rotate (1 orientation) then return false.
    /*
    if(activeBlock.getType() == ley::BlockNameType::cube) {
        return result.first;
    }
    */
    if(!mActiveBlock.getCanRotate()) {
        return result.first;    
    }

    while(!result.first && (kick > -MAX_KICK && kick < MAX_KICK)) {
        // TODO if we can't rotate then see which side of the board we are on.
        // then try moving in either direction to then see if its possible to rotate.
 
        //if the last rotate was blocked on a block or the bottom of the board.
        if(result.second == "block" || result.second == "board_bottom") {
            return false;
        }
        //Assume left or right side if the block position is on the left or right of center of the board.
        else {

            //assume left side
            //if kick is positive stay positive don't go back and forth between pos and neg.
            //If not rotateable do a wall kick to the left or right
            if(mActiveBlock.getPosRectPtr()->x < boardWidth/2 && kick >= 0) {
                kick += 1;
                moveBlock(ley::Command::right);
                SDL_Log("Kick right");
          
            }
            //assume right side
            else if(mActiveBlock.getPosRectPtr()->x >= boardWidth/2 && kick <= 0) {
                kick -= 1;
                moveBlock(ley::Command::left);
                SDL_Log("Kick left");
        
            }
        }

        result = rotateBlock(r);
    }

    return result.first;
}

std::pair<bool, std::string> ley::GameModel::rotateBlock(bool r) { // TODO, maybe ADD FEATURE - add control to Flip horz or vert.
    
    std::pair<bool, std::string> result = {false, ""};
    
    result = canRotate(r);
    if(result.first) {
        mActiveBlock.rotate(r);
        clearOldBlock();
        mOldBlock.rotate(r);
        // new board
        mBoard.putBlock(mActiveBlock);
        result.first = true;
    }

    return result;
}

std::pair<bool, std::string> ley::GameModel::canRotate(bool r) {
    //rotate the block if the game is not paused
    
    std::pair<bool, std::string> result = {false, ""};
    if(!isPaused() && mActiveBlock.rotate(r)) {
        result = mBoard.canPut(mActiveBlock, ley::Command::up);
        mActiveBlock.rotate(!r); //rotate it back, because this 
                                //function is simply a test only
    }
    
    return result;
}
void ley::GameModel::clearAndRecordLine(int lineNum) {

    ++mNumLines; //add to the score for each line
    //simply fill the rows where there are lines with the empty space.
    //new board
    mBoard.fillLine(lineNum, std::make_pair(ley::BlockTexCode::O, false));

    SDL_Log("Your Score is:%s" , std::to_string(mNumLines).c_str());
}
void ley::GameModel::shiftBoard(char start, char num) {
    //shift the board down start from the bottom right (backwards).
    char stopLine = start - num;
    for(char i = start; i >= 0; --i) {
        for(char j = mBoard.width() - 1; j >= 0; --j) {
            //new board
            if(i-num > -1) {
                mBoard.at(j,i)->first = mBoard.at(j,i-num)->first;
                mBoard.at(j,i)->second = mBoard.at(j,i-num)->second;
            }
        }
    }
}
void ley::GameModel::fillTop(char num) {
    for(char i = 0; i < num; i++) {
        //new board
        mBoard.fillLine(0, std::make_pair(ley::BlockTexCode::O, false));
    }
}
bool ley::GameModel::newLevel() {
    bool report = false;
    
    if(mNewLevelToReport) {
        report = true;
        mNewLevelToReport = false; //Flip back to false because we are going to pass along a true.
    }

    return report;
}

int ley::GameModel::calcLevel() {
    // TODO also need to account for the starting level option.
    int naturalLevel = (mNumLines / NEW_LVL_AT_LINES) + 1;
    return naturalLevel > mStartLevel ? naturalLevel : mStartLevel;
}

bool ley::GameModel::processLines(int &numLines) {
    //Check to see how many full lines we have starting from the bottom

    bool linesRemoved = false;

    std::vector<char> fullLines;
    //new board
    fullLines = checkForLines(mBoard.height() - 1 /*board.size()-1*/);
    numLines = fullLines.size();

    //Cut out one line at a time incase there are gaps in between the completed lines
    if(fullLines.size()>0) {
        //add to the score based on number of lines completed in a single move
        linesRemoved = true;
    }
    while(fullLines.size()>0) {
        //TODO does order matter for fullLines, should we remove the highest number(bottom) line first?
        clearAndRecordLine(fullLines.back());
        shiftBoard(fullLines.back(), 1);
        //Be sure to fill the top of the board again with clear values after shifting.
        fillTop(1);
        fullLines.pop_back();
    }

    const int newLevel = calcLevel();
    if(mNumLevel != newLevel) {
        mNumLevel = newLevel;
        mNewLevelToReport = true;
    }

    return linesRemoved;
}
//pass in start line and returns the line numbers that are full lines from the start line
std::vector<char> ley::GameModel::checkForLines(char start) {
    
    std::vector<char> fullLines;
    //check for consecutive lines.
    for(char i = start; i >= 0; --i){
        char currentLine = firstLineAt(i);
        if(currentLine != -1) {
            fullLines.push_back(currentLine);
            i = currentLine;
        }
    }

    return fullLines;
}

//return the first full line starting from start
int ley::GameModel::firstLineAt(int start) {
    bool fullline = false;

    for(int i = start; i >= 0; --i) {
        fullline = false;
        //new board
        for(int j = mBoard.width() - 1; j >= 0; --j) {
            if(!mBoard.at(j,i)->second) {
                fullline = false;
                break;
            } 
            else { fullline = true;}
        }
        if(fullline == true) { 
            SDL_Log("Full line made!!!"); return i;
        }
    }
    return -1;
}

void ley::GameModel::updateSpeed() {

    // Speed curve based on level — see https://www.desmos.com/calculator/gvmkmief5c
    static constexpr std::array<double, 32> SPEED_TABLE = {
        1000, // 0
        1000, // 1
         800, // 2  -200  (22.2%)
         624, // 3        (24.7%)
         487, // 4        (24.7%)
         380, // 5        (24.7%)
         297, // 6        (21.8%)
         232, // 7        (24.6%)
         181, // 8        (24.7%)
         142, // 9        (24.1%)
         111, // 10       (24.5%)
         100, // 11
          90, // 12
          80, // 13
          70, // 14
          60, // 15
          50, // 16
          45, // 17
          40, // 18
          35, // 19
          30, // 20
          25, // 21
          23, // 22
          21, // 23
          19, // 24
          17, // 25
          15, // 26
          14, // 27
          13, // 28
          12, // 29
          11, // 30
          10  // 31
    };

    const int level = calcLevel();
    if (level >= 0 && level < static_cast<int>(SPEED_TABLE.size())) {
        mCurrentSpeed = SPEED_TABLE[level];
    }
    // Beyond level 31 the speed stays at 10ms (already set at level 31)
}

//TODO this function should probably go in the controller
bool ley::GameModel::moveBlock(Command d) {

    if(mPaused) {
        return false; //don't do any moves while we are paused.
    }

    bool moved = false;

    switch (d) {
        case Command::down :
            //new block
            if (mBoard.canPut(mActiveBlock, Command::down).first) {
                mActiveBlock.moveDown(); //move the active block down
                clearOldBlock(); //Clear out the space where the active block was
                mOldBlock.moveDown(); //Move the oldBlock(clearer) down as well so it will clear correctly next time.
                moved = true;
            } 
            else { 
                // new board
                mBoard.setBlock(mActiveBlock);

                mAudioSystem.playSfx(ley::sfx::inplace);
                int currentLevel = mNumLevel; //Store the current level for points calculation which should happen for current level
                int turnLines = 0; //lines cleared for this single turn.
                if(processLines(turnLines)) {
                    onLine(turnLines, currentLevel);
                }
                else {
                    onDrop();
                }
                if(!newBlock()) {
                    setGameRunning(false);
                }
            }
        break;

        case Command::left :            
            //new board
            if (mBoard.canPut(mActiveBlock, Command::left).first) {
                mActiveBlock.moveLeft();
                clearOldBlock();
                mOldBlock.moveLeft();
                moved = true;
            }
        break;

        case Command::right :
            //new board
            if (mBoard.canPut(mActiveBlock, Command::right).first) {
                mActiveBlock.moveRight();
                clearOldBlock();
                mOldBlock.moveRight();
                moved = true;
            }
        break;

        default : ;
    }

    // new board
    mBoard.putBlock(mActiveBlock);
    
    updateSpeed();

    return moved;
}
void ley::GameModel::onDrop() {
    addToScore(PTS_DROP * mNumLevel);
    mComboCount = 0;
}
/* Use the level that the player is on while the lines are made */
void ley::GameModel::onLine(int lineCount, int level) {
    mAudioSystem.playSfx(ley::sfx::piecesfalling);

    int linesSameTime = 1;

    switch(lineCount) {
        case 2 : linesSameTime = PTS_2LINE_MULT;
        break;

        case 3 : linesSameTime = PTS_3LINE_MULT;
        break;

        case 4 : linesSameTime = PTS_4LINE_MULT;
        break;
    }

    addToScore(mPts_Line * level * linesSameTime * (mComboCount + 1));
    mComboCount++;
}
void ley::GameModel::overlayToggle() {
    mOverLayOn = !mOverLayOn;
}

bool ley::GameModel::isOverlayOn() {
    return mOverLayOn;
}

void ley::GameModel::setGameRunning(bool running) {
    mGameRunning = running;
}

void ley::GameModel::resetGame() {
    // new board
    mBoard.clear();
    setGameRunning(true);
    mNumLines = 0;
    mNumLevel = 1;
    mScore = 0;
    mComboCount = 0;
    initGame();
    mNewLevelToReport = true; //we always want to reset the game background when we restart the game.
    mNewHighScore = false;
    stopProgram(false);
}
bool ley::GameModel::isPaused() {
    return mPaused;
}

void ley::GameModel::pauseGame(bool pause) {
    mPaused = pause;
}

bool ley::GameModel::debugMode() {
    return mDebugMode;
}

void ley::GameModel::debugCommandsToggle() {
    mDebugMode = !mDebugMode;

    SDL_Log("Debug command keys have been turned %s", mDebugMode ? "on." : "off.");
}

bool ley::GameModel::debugOnlyLine() {
    return mDebugOnlyLine;
}

void ley::GameModel::debugOnlyLineToggle() {
    mDebugOnlyLine = !mDebugOnlyLine;
}

void ley::GameModel::newHighScore(bool flag) {
    mNewHighScore = flag;
}

bool ley::GameModel::newHighScore() {
    return mNewHighScore;
}

void ley::GameModel::quickDrop() {

    SDL_Log("Quick drop");
    while(ley::GameModel::moveBlock(ley::Command::down)) {

    }
}


