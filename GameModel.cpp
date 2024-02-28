/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/15/2020
*/
#include <array>
#include <SDL2/SDL.h>
#include "GameModel.h"
#include "Rand_int.h"

/* RAII */
ley::GameModel::GameModel()
: 
activeBlock(getRandomBlock()), 
oldBlock(activeBlock.getRect().x,activeBlock.getRect().y,activeBlock.getType(),1),
nextBlock(getRandomBlock()),
numLines(0),
numLevel(0),
gameRunning(true), 
currentSpeed(1000.0f), 
active(true), 
overlayOn(false),
score(0),
running(true),
mDebugOnlyLine(false)
{
   clearBoard();
   oldBlock.setH(activeBlock.getRect().h);
   oldBlock.setW(activeBlock.getRect().w);
   putBlock(activeBlock);

   mHighScores.read();
}

ley::GameModel::~GameModel() {

}

/* Accessors */
ley::Block ley::GameModel::getNextBlock() {
    return nextBlock;
}
bool ley::GameModel::programRunning() {
    return running;
}

void ley::GameModel::stopProgram(bool stop) {
    running = !stop;
}

bool ley::GameModel::isGameRunning() {
    return gameRunning;
}


int ley::GameModel::getScore() {
    return score;
}

int ley::GameModel::getLines() {
    return numLines;
}

int ley::GameModel::getLevel() {
    return numLevel;
}

std::array<std::array<std::pair<ley::BlockTexCode,bool>, ley::BOARDSIZE_WIDTH>, ley::BOARDSIZE_HEIGHT >*
ley::GameModel::getBoard() {
    return &board;
}

/* Functions */

void ley::GameModel::addToScore(long p) {
    score+=p;
}

void ley::GameModel::clearBoard() {
    for(auto i = 0; i < BOARDSIZE_HEIGHT; ++i) {
        board[i].fill(std::make_pair(BlockTexCode::O,false));
    }
}

void ley::GameModel::debugResetActiveBlock() {
    
    activeBlock = debugGetBlock();
    oldBlock = activeBlock;
    oldBlock.setClear(true);
    activeBlock.debugResetPos();
    oldBlock.debugResetPos();
}

void ley::GameModel::debugFill() {
    
    board[22].fill(std::make_pair(BlockTexCode::d, true));
    board[22][0] = std::make_pair(BlockTexCode::O, false);

    board[21].fill(std::make_pair(BlockTexCode::e, true));
    board[21][0] = std::make_pair(BlockTexCode::O, false);
    board[21][1] = std::make_pair(BlockTexCode::O, false);

    board[20].fill(std::make_pair(BlockTexCode::d, true));
    board[20][0] = std::make_pair(BlockTexCode::O, false);
}

void ley::GameModel::putBlock(Block& b) { //put the active block onto the board.
    SDL_Rect rect = b.getRect();
    for(int i = 0; i < rect.w; ++i) {
        for(int j = 0; j < rect.h; ++j) {
            // NOTE board works backwards, this is where we reverse x and y. 
            //      this is common, see:
            // https://stackoverflow.com/questions/2203525/are-the-x-y-and-row-col-attributes-of-a-two-dimensional-array-backwards 
            //
            BlockTexCode renderPart = b.renderPart(i,j);
            if(board[rect.y+j][rect.x+i].second == false) {
                board[rect.y+j][rect.x+i].first = renderPart;
            }
        }
    }
}

// **check for collision with the other blocks that have been put in place, 
//   also checks for collision with game board boundry
bool ley::GameModel::canPut(Block& b, Command d) {

    SDL_Rect block = b.getRect();

    // TODO these 4 nested loops can probably be combined into one as there are only minor differences.
    //Iterate through the block and check and see if the board is empty where we need to put a block part.
    //Check if there is already a block along the edge of the existing block.
    switch (d) {
        case Command::down : 
           for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) {
                    
                    bool renderPart = b.renderPart(i, j) != BlockTexCode::O; // part to render
                    bool boardPart = board[block.y + j + 1][block.x + i].second
                                        || block.y + j + 1 >= BOARDSIZE_HEIGHT; //the space is already occupied
                    if(renderPart && boardPart) {
                            return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        case Command::right : 
            for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) {
                    bool boardPart = board[block.y + j][block.x + i + 1].second
                                    || ((block.x + 1) + i) > (BOARDSIZE_WIDTH - 1);
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                           return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        case Command::left :
            for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) { 
                    bool boardPart = board[block.y + j][ (block.x - 1) + i ].second
                                    || ((block.x - 1) + i) < 0;
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                            return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        case Command::up : //this is a rotation
            for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) {
                    //if there is a block piece to put,then check to see if it can be put.
                    bool boardPart = board[block.y + j][block.x + i].second //board empty.
                                    || block.y + j < 0 || block.y + j > BOARDSIZE_HEIGHT //above the board
                                    || block.x + i > BOARDSIZE_WIDTH - 1 //right of the board
                                    || block.x + i < 0; // left of the board
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                            return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        default : break;
    }

    return true;
}

void ley::GameModel::clearOldBlock() {
    putBlock(oldBlock);
}
ley::Block ley::GameModel::debugGetBlock() {

    return Block(4,0,BlockType::tee,false);
}

ley::Block ley::GameModel::getRandomBlock() {
    Block a;
    ley::Rand_int rand0to6(0,6); //random number generator
    
    if(mDebugOnlyLine == true) { //debug return only line
        return Block(4,2,BlockType::line,false);
    }

    switch(rand0to6()) {
        case 0 : 
            a = Block(4,0,BlockType::tee,false);
        break;
        case 1 :
            a = Block(4,0,BlockType::rLee,false);
        break;
        case 2 : 
            a = Block(4,1,BlockType::zee,false);
        break;
        case 3 : 
            a = Block(4,1,BlockType::mzee,false);
        break;
        case 4 : 
            a = Block(4,0,BlockType::lLee,false);
        break;
        case 5 :
            a = Block(4,2,BlockType::line,false);
        break;
        case 6 :
            a = Block(4,1,BlockType::cube,false);
        break;
    }

    return a;
}

bool ley::GameModel::newBlock() {
    SDL_Log("New Block");

    activeBlock = nextBlock;
    oldBlock = activeBlock;
    oldBlock.setClear(true);

    nextBlock = getRandomBlock();

    //check if we can put down the new active block.
    if(!canPut(activeBlock, ley::Command::up)) {
        return false;
    }
    else {
        putBlock(activeBlock);
        return true;
    }
   
    return false;
}

//Iterate through the height and width of the block and set
//the board action layer elements to isset.
void ley::GameModel::setBlock() {
    SDL_Rect blockRect = activeBlock.getRect();
    for(auto i=0; i<blockRect.w; ++i) {
        for(auto j=0; j<blockRect.h; ++j) {
            if(activeBlock.renderPart(i,j) != BlockTexCode::O) {
                board[blockRect.y + j][blockRect.x + i].second = true;
            }
        }
    }
}
bool ley::GameModel::rotateBlock(bool r) { // TODO, maybe ADD FEATURE - add control to Flip horz or vert.
    
    bool rotated = false;
    
    if(canRotate(r)) {
        activeBlock.rotate(r);
        clearOldBlock();
        oldBlock.rotate(r);
        putBlock(activeBlock);
        rotated = true;
    }

    return rotated;
}

bool ley::GameModel::canRotate(bool r) {
    //rotate the block if the game is not paused
    bool canput = false;

    if(!isPaused() && activeBlock.rotate(r)) {
        canput = canPut(activeBlock, ley::Command::up); //up is a rotation
        activeBlock.rotate(!r); //rotate it back, because this 
                                //function is simply a test only
    }
    
    return canput;
}
void ley::GameModel::clearAndRecordLine(int lineNum) {

    ++numLines; //add to the score for each line
    //simply fill the rows where there are lines with the empty space.
    board[lineNum].fill(std::make_pair(ley::BlockTexCode::O, false));

    SDL_Log("Your Score is:%s" , std::to_string(numLines).c_str());
}
void ley::GameModel::shiftBoard(char start, char num) {
    //shift the board down start from the bottom right (backwards).
    char stopLine = start - num;
    for(char i = start; i >= 0; --i) {
        for(char j = board[i].size() - 1; j >= 0; --j) {
            board[i][j] = board[i-num][j];
        }
    }
}
void ley::GameModel::fillTop(char num) {
    for(char i = 0; i < num; i++) {
        board[i].fill(std::make_pair(BlockTexCode::O,false));
    }
}
bool ley::GameModel::newLevel() {
    bool report = false;
    
    if(newLevelToReport) {
        report = true;
        newLevelToReport = false; //Flip back to false because we are going to pass along a true.
    }

    return report;
}

int ley::GameModel::calcLevel() {
    return numLines / NEW_LVL_AT_LINES;
}

bool ley::GameModel::processLines() {
    //Check to see how many full lines we have starting from the bottom

    bool linesRemoved = false;

    std::vector<char> fullLines;

    fullLines = checkForLines(board.size()-1);

    //Cut out one line at a time incase there are gaps in between the completed lines
    if(fullLines.size()>0) {
        //add to the score based on number of lines completed in a single move
        addToScore((fullLines.size() * (numLevel+1)) * 10 );
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

    if(numLevel != calcLevel()) {
        numLevel = calcLevel();
        newLevelToReport = true;
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
        for(int j = board[i].size() - 1; j >= 0; --j) {
            if(!board[i][j].second) {
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

    //a much more natural curve for speed increases.
    // original https://www.desmos.com/calculator/7xuo8jryb9
    // percent  https://www.desmos.com/calculator/hivnmiyedk

    switch(numLevel) {
        case 0 : currentSpeed = 1000;
            break;
        case 1 : currentSpeed = 800; //-200 22.2222% 
            break;
        case 2 : currentSpeed = 624; // 24.7191%
            break;
        case 3 : currentSpeed = 487; // 24.6624%
            break;
        case 4 : currentSpeed = 380; // 24.6828%
            break;
        case 5 : currentSpeed = 297; // 21.8421%
            break;
        case 6 : currentSpeed = 232; // 24.5746%
            break;
        case 7 : currentSpeed = 181; // 24.6973%
            break;
        case 8 : currentSpeed = 142; // 24.148%
            break;
        case 9 : currentSpeed = 111; // 24.5059%
    }
}

//TODO this function should probably go in the controller
bool ley::GameModel::moveBlock(Command d) {

    if(!active) {
        return false; //don't do any moves while we are paused.
    }

    bool moved = false;

    switch (d) {
        case Command::down :
            if (canPut(activeBlock, Command::down)) {
                activeBlock.moveDown(); //move the active block down
                clearOldBlock(); //Clear out the space where the active block was
                oldBlock.moveDown(); //Move the oldBlock(clearer) down as well so it will clear correctly next time.
                moved = true;
            } 
            else { 
                setBlock();
                audSystem.playSfx(ley::sfx::inplace);
                if(processLines()) {
                    audSystem.playSfx(ley::sfx::piecesfalling);
                }
                if(!newBlock()) {
                    setGameRunning(false);
                }
            }
        break;

        case Command::left :
            if (canPut(activeBlock, Command::left)) {
                activeBlock.moveLeft();
                clearOldBlock();
                oldBlock.moveLeft();
                moved = true;
            }
        break;

        case Command::right :
            if (canPut(activeBlock, Command::right)) {
                activeBlock.moveRight();
                clearOldBlock();
                oldBlock.moveRight();
                moved = true;
            }
        break;

        default : ;
    }

    putBlock(activeBlock);
    updateSpeed();

    return moved;
}
void ley::GameModel::overlayToggle() {
    overlayOn = !overlayOn;
}

bool ley::GameModel::isOverlayOn() {
    return overlayOn;
}

void ley::GameModel::debugBoard(bool setLayer) {
    
    SDL_Log("Debug Board");
    std::string sRow = "";
    for(auto row : board) {
            sRow = ""; //clear
        for(auto column : row) {
            if(setLayer) {
                sRow += std::to_string(column.second) + ".";
            } else {
                sRow += TEXCODE_CHAR.find(column.first)->second + ".";
            }
        }
        SDL_Log("%s",sRow.c_str());
    }
}
void ley::GameModel::setGameRunning(bool running) {
    gameRunning = running;
}

void ley::GameModel::resetGame() {
    clearBoard();
    setGameRunning(true);
    numLines = 0;
    numLevel = 0;
    score = 0;

    activeBlock.reset();
    oldBlock.reset(); //this is the old position that gets cleaned up when the block moves, this needs to be reset to.
    newLevelToReport = true; //we always want to reset the game background when we restart the game.
    mNewHighScore = false;
    stopProgram(false);   
}
bool ley::GameModel::isPaused() {
    return !active;
}

void ley::GameModel::pauseGame(bool pause) {
        
        active = !pause;
}

bool ley::GameModel::debugCommands() {
    return debugCommandsFlag;
}

void ley::GameModel::debugCommandsToggle() {
    debugCommandsFlag = !debugCommandsFlag;

    SDL_Log("Debug command keys have been turned %s", debugCommandsFlag ? "on." : "off.");
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

ley::UIFocusChange ley::GameModel::UIInputFocus() {
    return mUIFocusChange;
}

void ley::GameModel::UIInputFocus(ley::UIFocusChange fc) {
    mUIFocusChange = fc;
}