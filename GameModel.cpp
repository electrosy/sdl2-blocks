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
gameOver(false), 
currentSpeed(1000.0f), 
active(true), 
overlayOn(false),
score(0),
running(true),
frame_count(0),
avg_fps(0)
{
   clearBoard();
   oldBlock.setH(activeBlock.getRect().h);
   oldBlock.setW(activeBlock.getRect().w);
   putBlock(activeBlock);

   audSystem.playIntro();
}

ley::GameModel::~GameModel() {

}

/* Accessors */
Uint32 ley::GameModel::avgFPS() {

    if(SDL_GetTicks() % 1000 == 0) { //recalculate the AVG fps only once per second.
        avg_fps = secondsFromStart() == 0 ? secondsFromStart() : frameCount() / secondsFromStart();
        SDL_Log("GameModel::frameCount(): %u", frameCount());
        SDL_Log("GameModel::secondsFromStart(): %u", secondsFromStart());
        SDL_Log("GameModel::avgFPS(): %u", avg_fps);
    }

    return avg_fps;
}

void ley::GameModel::resetClock() {
    mainClock.reset();
}

Uint32 ley::GameModel::secondsFromStart() {
    return mainClock.secondsFromStart();
}
void ley::GameModel::setTimer(ley::Timer* pT) {
    ptrTimer = pT;
}

ley::Block ley::GameModel::getNextBlock() {
    return nextBlock;
}
bool ley::GameModel::programRunning() {
    return running;
}

void ley::GameModel::stopProgram(bool stop) {
    running = !stop;
}

bool ley::GameModel::isGameOver() {
    return gameOver;
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

size_t ley::GameModel::frameCount() {
    return frame_count;
}

/* Functions */
void ley::GameModel::frameCountInc() {
    ++frame_count;
}

void ley::GameModel::addToScore(long p) {
    score+=p;
}

void ley::GameModel::clearBoard() {
    for(auto i = 0; i < BOARDSIZE_HEIGHT; ++i) {
        board[i].fill(std::make_pair(BlockTexCode::O,false));
    }
}

void ley::GameModel::fadeMusic() {
    audSystem.fadeOutMusic();
}

void ley::GameModel::playMainMenu() {
    audSystem.playMainMenu();
}

void ley::GameModel::startPlayList() {
    audSystem.playPlaylist();
}

void ley::GameModel::playNext() {
    audSystem.playNext();
}

// TODO This function should probably go in the controller?
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
bool ley::GameModel::canPut(Block& b, Direction d) { // TODO canput() should probably go into the game controller
/* Check borders */
    unsigned int offset_x = 0;
    unsigned int offset_y = 0;
    SDL_Rect rect = b.getRect();


    switch (d) {
        case Direction::down :
          offset_x = 0;
          offset_y = rect.h + 1;
        break;
        case Direction::right :
            offset_x = rect.w + 1;
            offset_y = 0;
        break;
        case Direction::left :
            offset_x = -1;
            offset_y = 0;
        break;
        case Direction::up : //rotation
            offset_x = rect.w;
            offset_y = rect.h;
 
        default : break;
    }


    //if asked to go outside the bounds of the game board.
    if( (rect.x+offset_x > BOARDSIZE_WIDTH) || (rect.y+offset_y > BOARDSIZE_HEIGHT) ) {
       return false; /*!*! EARLY EXIT !*!*/  //hit the border.
    } /* End Check Borders */

    //Iterate through the block and check and see if the board is empty where we need to put a block part.
    //Check if there is already a block along the edge of the existing block.
    switch (d) {
        case Direction::down : 
           for(int i = 0; i < rect.w; ++i) {
               for(int j = 0; j < rect.h; ++j) {
                    //if there is a block piece to put,then check to see if it can be put.
                    BlockTexCode renderablePart = b.renderPart(i, j);
                    BlockTexCode boardPart = board[rect.y + b.heightAtWidth(i) + 1][rect.x + i].first;
                    if( (renderablePart != BlockTexCode::O)//we have a part to render.
                        && (boardPart != BlockTexCode::O)//there is an empty space on the board.
                    ) {
                            return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        case Direction::right : 
            for(int i = 0; i < rect.w; ++i) {
               for(int j = 0; j < rect.h; ++j) {
                    //if there is a block piece to put,then check to see if it can be put.
                    bool boardPart = board[rect.y + j][rect.x + i + 1].second;
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                           return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        case Direction::left :
            for(int i = 0; i < rect.w; ++i) {
               for(int j = 0; j < rect.h; ++j) {
                    //if there is a block piece to put,then check to see if it can be put.
                    bool boardPart = board[rect.y + j][ (rect.x - 1) + i ].second;
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                            return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        case Direction::up : //this is a rotation
            for(int i = 0; i < rect.w; ++i) {
               for(int j = 0; j < rect.h; ++j) {
                    //if there is a block piece to put,then check to see if it can be put.
                    bool boardPart = board[rect.y + j][rect.x + i].second;
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                            return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break; // TODO extra break??
        default : break;
    }

    return true;
}

void ley::GameModel::downExpired() {

}
void ley::GameModel::clearOldBlock() {
    putBlock(oldBlock);
}

ley::Block ley::GameModel::getRandomBlock() {
    Block a;
    ley::Rand_int rand0to6(0,6); //random number generator
    
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
    if(!canPut(activeBlock, ley::Direction::up)) {
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
void ley::GameModel::rotateBlock(bool r) {
    if(canRotate(r)) {
        activeBlock.rotate(r);
        clearOldBlock();
        oldBlock.rotate(r);
        putBlock(activeBlock);
    }
}

//** TODO we need to synchronize the rotation so that it doesn't happen 
//** TODO when the timer moves the block down.
bool ley::GameModel::canRotate(bool r) {
    //rotate the block if the game is not paused
    bool canput = false;

    if(!isPaused()) {
        activeBlock.rotate(r);
        canput = canPut(activeBlock, ley::Direction::up); //up is a rotation
        activeBlock.rotate(!r); //rotate it back, because this 
                                //function is simply a test only
    }
    
    return canput;
}
void ley::GameModel::clearAndRecordLines(int first, int last) {
    for(int i = first; i >= last; --i) {
        ++numLines; //add to the score for each line
        //If numLines is a multiple of 10 then increase the level as well
    
        //simply fill the rows where there are lines with the empty space.
        board[i].fill(std::make_pair(ley::BlockTexCode::O, false));
    }
    SDL_Log("Your Score is:%s" , std::to_string(numLines).c_str());
}
void ley::GameModel::shiftBoard(char start, char num) {
    //shift the board down start from the bottom right (backwards).
    char stopLine = start - num;
    for(char i = start; i >= 0; --i) {
        for(char j = board[i].size() -1; j >= 0; --j) {
            board[i][j] = board[i-num][j];
        }
    }
}
void ley::GameModel::fillTop(char num) {
    for(char i = 0; i < num; i++) {
        board[i].fill(std::make_pair(BlockTexCode::O,false));
    }
}

void ley::GameModel::processLines() {
    //Check to see how many full lines we have starting from the bottom
    std::pair<char,char> firstAndLast;
    firstAndLast.first = -1;
    firstAndLast.second = -1;
    firstAndLast = checkForLines(board.size()-1);

    //remove the lines if neither of them are -1;
    if(firstAndLast.first != -1 && firstAndLast.second != -1) {
        //Shift the board down the appropriate number of spaces.
        char linesToCut = firstAndLast.first - firstAndLast.second + 1;
        clearAndRecordLines(firstAndLast.first, firstAndLast.second);
        addToScore( (linesToCut * (numLevel+1)) * 10 );
        if(numLines % NEW_LVL_AT_LINES == 0) { // new level every X lines.
            ++numLevel;
        //also increase the speed of the fall down timer
        this->ptrTimer->changeSpeed(500); //for testing just double the speed.
    }
        
        shiftBoard(firstAndLast.first, linesToCut);
        //Be sore to fill the top of the board again with clear values after shifting.
        fillTop(linesToCut);

        //add to the score based on number of lines completed in a single move 1 to 4 (i dont think this could ever be more than four)
        
    }

    

    

    // TODO also continue to check the rest of the board for full lines. (but is this a valid case?)
}

//pass in start line and returns the number of lines that are full lines from the start line
std::pair<char,char> ley::GameModel::checkForLines(char start) {
    
    char firstLine = firstLineAt(start);
    char lastLine = -1;
    char counterStart = firstLine != -1 ? firstLine : start;
    //check for consecutive lines.
    for(int i = counterStart; i >= 0; --i){
        int currentLine = firstLineAt(i);
        if(currentLine != -1) {
            lastLine = currentLine;
        }
    }

    SDL_Log("First line: %s -> Last line: %s",std::to_string(firstLine).c_str(), std::to_string(lastLine).c_str());
    return std::make_pair(firstLine,lastLine);
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

    switch(numLevel) {
        case 0 : currentSpeed = 1000;
            break;
        case 1 : currentSpeed = 900;
            break;
        case 2 : currentSpeed = 800;
            break;
        case 3 : currentSpeed = 700;
            break;
        case 4 : currentSpeed = 600;
            break;
        case 5 : currentSpeed = 500;
            break;
        case 6 : currentSpeed = 400;
            break;
        case 7 : currentSpeed = 300;
            break;
        case 8 : currentSpeed = 200;
            break;
        case 9 : currentSpeed = 100;
    }
}

//TODO this function should probably go in the controller
double ley::GameModel::moveBlock(Direction d) {

    if(!active) {
        return currentSpeed; //don't do any movies while we are paused.
    }

    switch (d) {
        case Direction::down :
            if (canPut(activeBlock, Direction::down)) {
                activeBlock.moveDown(); //move the active block down
                clearOldBlock(); //Clear out the space where the active block was
                oldBlock.moveDown(); //Move the oldBlock(clearer) down as well so it will clear correctly next time.
            } 
            else { 
                setBlock();
                processLines();
                if(!newBlock()) {
                    gameOver = true;
                }
            }
        break;

        case Direction::left :
            if (canPut(activeBlock, Direction::left)) {
                activeBlock.moveLeft();
                clearOldBlock();
                oldBlock.moveLeft();
            }
        break;

        case Direction::right :
            if (canPut(activeBlock, Direction::right)) {
                activeBlock.moveRight();
                clearOldBlock();
                oldBlock.moveRight();
            }
        break;

        default : ;
    }

    putBlock(activeBlock);
    updateSpeed();

    return currentSpeed;
}
void ley::GameModel::overlayToggle() {
    overlayOn = !overlayOn;
}

bool ley::GameModel::isOverlayOn() {
    return overlayOn;
}

void ley::GameModel::debugBoard(bool setLayer) {
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

void ley::GameModel::resetGame() {
    clearBoard();
    gameOver = false;
    numLines = 0;
    numLevel = 0;
    score = 0;

    activeBlock.reset();
    oldBlock.reset(); //this is the old position that gets cleaned up when the block moves, this needs to be reset to.

    audSystem.fadeOutMusic();
    stopProgram(false);
    
}
bool ley::GameModel::isPaused() {
    return !active;
}

void ley::GameModel::pauseGame(bool pause) {
        
        active = !pause;
}