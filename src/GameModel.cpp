/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/15/2020
*/
#include <array>
#include <iostream>

#include <SDL2/SDL.h>
#include "../inc/GameModel.h"
#include "../Rand_int.h"


/* RAII */
ley::GameModel::GameModel()
:
numLines(0),
numLevel(1),
gameRunning(true), 
currentSpeed(1000.0f), 
active(true), 
overlayOn(false),
score(0),
running(true),
mDebugOnlyLine(false)
{
    readBlockData();
    ley::Block::setBlockDataPtr(&mBlockMapData);
    
    initGame();

    SDL_Log("ley::GameModel::ctor");
    oldBlock.setH(activeBlock.getRect().h);
    oldBlock.setW(activeBlock.getRect().w);
      
    mHighScores.read();
    
    loadKeyBindings();
    loadButtonBindings();

    readConfig();
    // TODO add the board size to the mainconfig
    readConfigOther();

    activeBlock.setBlockDataPtr(&mBlockMapData);
    
    mLanguageModel.loadLanguage();

}

// TODO rule of 3/5
ley::GameModel::~GameModel() {
    
    writeConfig();
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

ley::Board* ley::GameModel::getBoard() {
    return &mBoard;
}

/* Functions */
void ley::GameModel::addToScore(long p) {
    score += p;
}

void ley::GameModel::debugResetActiveBlock() {
    
    activeBlock = debugGetBlock();
    oldBlock = activeBlock;
    oldBlock.setClear(true);
    activeBlock.debugResetPos();
    oldBlock.debugResetPos();
}

void ley::GameModel::clearOldBlock() {
    //new board
    mBoard.putBlock(oldBlock);
}
ley::Block ley::GameModel::debugGetBlock() {

    return Block(4,0,BlockNameType::tee,false);
}

ley::Block ley::GameModel::getRandomBlock() {
    Block a;
    ley::Rand_int rand0to6(0,6); //random number generator
    Uint8 yStartPos = 2;

    if(mDebugOnlyLine == true) { //debug return only line
        return Block(4,2,BlockNameType::line,false);
    }

    switch(rand0to6()) {
        case 0 : 
            a = Block(4,yStartPos,BlockNameType::tee,false);
        break;
        case 1 :
            a = Block(4,yStartPos,BlockNameType::rLee,false);
        break;
        case 2 : 
            a = Block(4,yStartPos,BlockNameType::zee,false);
        break;
        case 3 : 
            a = Block(4,yStartPos,BlockNameType::mzee,false);
        break;
        case 4 : 
            a = Block(4,yStartPos,BlockNameType::lLee,false);
        break;
        case 5 :
            a = Block(4,yStartPos,BlockNameType::line,false);
        break;
        case 6 :
            a = Block(4,yStartPos,BlockNameType::cube,false);
        break;
    }

    //set the position so the bottom of the block will appear at the top of the board regardless of block size or shape
    a.getPosRectPtr()->y = a.getPosRectPtr()->y - a.getPosRectPtr()->h;

    return a;
}

bool ley::GameModel::newBlock() {
    SDL_Log("New Block");

    activeBlock = nextBlock;
    oldBlock = activeBlock;
    oldBlock.setClear(true);
    nextBlock = getRandomBlock();

    //check if we can put down the new active block.
    if(!mBoard.canPut(activeBlock, ley::Command::up).first) {
        return false;
    }
    else {
        mBoard.putBlock(activeBlock);
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
    if(!activeBlock.getCanRotate()) {
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
            if(activeBlock.getPosRectPtr()->x < boardWidth/2 && kick >= 0) {
                kick += 1;
                moveBlock(ley::Command::right);
                SDL_Log("Kick right");
          
            }
            //assume right side
            else if(activeBlock.getPosRectPtr()->x >= boardWidth/2 && kick <= 0) {
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
        activeBlock.rotate(r);
        clearOldBlock();
        oldBlock.rotate(r);
        // new board
        mBoard.putBlock(activeBlock);
        result.first = true;
    }

    return result;
}

std::pair<bool, std::string> ley::GameModel::canRotate(bool r) {
    //rotate the block if the game is not paused
    
    std::pair<bool, std::string> result = {false, ""};
    if(!isPaused() && activeBlock.rotate(r)) {
        result = mBoard.canPut(activeBlock, ley::Command::up);
        activeBlock.rotate(!r); //rotate it back, because this 
                                //function is simply a test only
    }
    
    return result;
}
void ley::GameModel::clearAndRecordLine(int lineNum) {

    ++numLines; //add to the score for each line
    //simply fill the rows where there are lines with the empty space.
    //new board
    mBoard.fillLine(lineNum, std::make_pair(ley::BlockTexCode::O, false));

    SDL_Log("Your Score is:%s" , std::to_string(numLines).c_str());
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
    
    if(newLevelToReport) {
        report = true;
        newLevelToReport = false; //Flip back to false because we are going to pass along a true.
    }

    return report;
}

int ley::GameModel::calcLevel() {
    return (numLines / NEW_LVL_AT_LINES) + 1;
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

    //a much more natural curve for speed increases.
    // original https://www.desmos.com/calculator/7xuo8jryb9
    // percent  https://www.desmos.com/calculator/hivnmiyedk

    switch(numLevel) {
        case 0 :
        case 1 : currentSpeed = 1000;
            break;
        case 2 : currentSpeed = 800; //-200 22.2222% 
            break;
        case 3 : currentSpeed = 624; // 24.7191%
            break;
        case 4 : currentSpeed = 487; // 24.6624%
            break;
        case 5 : currentSpeed = 380; // 24.6828%
            break;
        case 6 : currentSpeed = 297; // 21.8421%
            break;
        case 7 : currentSpeed = 232; // 24.5746%
            break;
        case 8 : currentSpeed = 181; // 24.6973%
            break;
        case 9 : currentSpeed = 142; // 24.148%
            break;
        case 10 : currentSpeed = 111; // 24.5059%
    }
}

//TODO this function should probably go in the controller
bool ley::GameModel::moveBlock(Command d) {

    // TODO, should we just not call moveBlock at all?
    if(!active) {
        return false; //don't do any moves while we are paused.
    }

    bool moved = false;

    switch (d) {
        case Command::down :
            //new block
            if (mBoard.canPut(activeBlock, Command::down).first) {
                activeBlock.moveDown(); //move the active block down
                clearOldBlock(); //Clear out the space where the active block was
                oldBlock.moveDown(); //Move the oldBlock(clearer) down as well so it will clear correctly next time.
                moved = true;
            } 
            else { 
                // new board
                mBoard.setBlock(activeBlock);

                audSystem.playSfx(ley::sfx::inplace);
                int currentLevel = numLevel; //Store the current level for points calculation which should happen for current level
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
            if (mBoard.canPut(activeBlock, Command::left).first) {
                activeBlock.moveLeft();
                clearOldBlock();
                oldBlock.moveLeft();
                moved = true;
            }
        break;

        case Command::right :
            //new board
            if (mBoard.canPut(activeBlock, Command::right).first) {
                activeBlock.moveRight();
                clearOldBlock();
                oldBlock.moveRight();
                moved = true;
            }
        break;

        default : ;
    }

    // new board
    mBoard.putBlock(activeBlock);
    
    updateSpeed();

    return moved;
}
void ley::GameModel::onDrop() {
    addToScore(PTS_DROP * numLevel);
    mComboCount = 0;
}
/* Use the level that the player is on while the lines are made */
void ley::GameModel::onLine(int lineCount, int level) {
    audSystem.playSfx(ley::sfx::piecesfalling);

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
    overlayOn = !overlayOn;
}

bool ley::GameModel::isOverlayOn() {
    return overlayOn;
}

void ley::GameModel::setGameRunning(bool running) {
    gameRunning = running;
}

void ley::GameModel::resetGame() {
    // new board
    mBoard.clear();

    setGameRunning(true);
    numLines = 0;
    numLevel = 1;
    score = 0;
    mComboCount = 0;

    activeBlock.reset();
    oldBlock.reset(); //this is the old position that gets cleaned up when the block moves, this needs to be reset to.

    initGame();
    /*
    // Ensure that we have the latest block data if we start a new game right after mading modifications in the block editor.
    SDL_Rect tempRect;
    Block::setBlockDataFromFile(activeBlock.getType(), activeBlock.getBlockOrientation(), activeBlock.getBlockDataPtr(), false, &tempRect,{});
    activeBlock.setH(tempRect.h);
    activeBlock.setW(tempRect.w);
    Block::setBlockDataFromFile(oldBlock.getType(), oldBlock.getBlockOrientation(), oldBlock.getBlockDataPtr(), true, &tempRect,{});
    oldBlock.setH(tempRect.h);
    oldBlock.setW(tempRect.w);
    */
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

void ley::GameModel::readConfigOther() {
    mConfig.read();

    if(mConfig.height() >= BOARDHEIGHT_MIN and mConfig.width() >= BOARDWIDTH_MIN) {
        resizeBoard(mConfig.width(), mConfig.height());
    }
}

void ley::GameModel::resizeBoard(int width, int height) {

    mBoard.setSize(width, height + BOARDSIZE_BUFFER);
    mPts_Line = mBoard.width() * (PTS_DROP * 2);
    mBoard.putBlock(activeBlock);
}

void ley::GameModel::loadKeyBindings() {

    mKeyBindings.emplace(SDL_SCANCODE_GRAVE, std::make_pair(KMOD_NONE, ley::Command::console));

    mKeyBindings.emplace(SDL_SCANCODE_F12, std::make_pair(KMOD_NONE, ley::Command::debugkeystoggle));

    mKeyBindings.emplace(SDL_SCANCODE_LEFT, std::make_pair(KMOD_NONE, ley::Command::left));
    mKeyBindings.emplace(SDL_SCANCODE_LEFT, std::make_pair(KMOD_ALT, ley::Command::shiftleft));
    mKeyBindings.emplace(SDL_SCANCODE_LEFT, std::make_pair(KMOD_SHIFT, ley::Command::shiftmajleft));
    mKeyBindings.emplace(SDL_SCANCODE_LEFT, std::make_pair(KMOD_CTRL, ley::Command::shiftallleft));

    mKeyBindings.emplace(SDL_SCANCODE_RIGHT, std::make_pair(KMOD_NONE, ley::Command::right));
    mKeyBindings.emplace(SDL_SCANCODE_RIGHT, std::make_pair(KMOD_ALT, ley::Command::shiftright));
    mKeyBindings.emplace(SDL_SCANCODE_RIGHT, std::make_pair(KMOD_SHIFT, ley::Command::shiftmajright));
    mKeyBindings.emplace(SDL_SCANCODE_RIGHT, std::make_pair(KMOD_CTRL, ley::Command::shiftallright));

    mKeyBindings.emplace(SDL_SCANCODE_DOWN, std::make_pair(KMOD_NONE, ley::Command::down));
    mKeyBindings.emplace(SDL_SCANCODE_DOWN, std::make_pair(KMOD_SHIFT, ley::Command::shiftmajdown));
    mKeyBindings.emplace(SDL_SCANCODE_DOWN, std::make_pair(KMOD_ALT, ley::Command::shiftdown));
    mKeyBindings.emplace(SDL_SCANCODE_DOWN, std::make_pair(KMOD_CTRL, ley::Command::shiftalldown));

    mKeyBindings.emplace(SDL_SCANCODE_UP, std::make_pair(KMOD_NONE, ley::Command::cclockwise));
    mKeyBindings.emplace(SDL_SCANCODE_UP, std::make_pair(KMOD_ALT, ley::Command::shiftup));
    mKeyBindings.emplace(SDL_SCANCODE_UP, std::make_pair(KMOD_SHIFT, ley::Command::shiftmajup));
    mKeyBindings.emplace(SDL_SCANCODE_UP, std::make_pair(KMOD_CTRL, ley::Command::shiftallup));

    mKeyBindings.emplace(SDL_SCANCODE_E, std::make_pair(KMOD_NONE, ley::Command::cclockwise));
    
    mKeyBindings.emplace(SDL_SCANCODE_R, std::make_pair(KMOD_NONE, ley::Command::clockwise));

    mKeyBindings.emplace(SDL_SCANCODE_Q, std::make_pair(KMOD_NONE, ley::Command::quit));
    mKeyBindings.emplace(SDL_SCANCODE_ESCAPE, std::make_pair(KMOD_NONE, ley::Command::quit));

    mKeyBindings.emplace(SDL_SCANCODE_RETURN, std::make_pair(KMOD_NONE, ley::Command::enter));
    mKeyBindings.emplace(SDL_SCANCODE_RETURN, std::make_pair(KMOD_ALT, ley::Command::fullscreen));

    mKeyBindings.emplace(SDL_SCANCODE_N, std::make_pair(KMOD_NONE, ley::Command::nextSong));

    mKeyBindings.emplace(SDL_SCANCODE_B, std::make_pair(KMOD_NONE, ley::Command::previousSong));

    mKeyBindings.emplace(SDL_SCANCODE_SPACE, std::make_pair(KMOD_NONE, ley::Command::space));

    mKeyBindings.emplace(SDL_SCANCODE_P, std::make_pair(KMOD_NONE, ley::Command::pause));
    mKeyBindings.emplace(SDL_SCANCODE_SLASH, std::make_pair(KMOD_NONE, ley::Command::pause));

    
    mKeyBindings.emplace(SDL_SCANCODE_MINUS, std::make_pair(KMOD_NONE, ley::Command::decreaseVolume));
    mKeyBindings.emplace(SDL_SCANCODE_MINUS, std::make_pair(KMOD_RSHIFT, ley::Command::decreaseTransparency));
    
    mKeyBindings.emplace(SDL_SCANCODE_KP_MINUS, std::make_pair(KMOD_NONE, ley::Command::decreaseVolume));

    mKeyBindings.emplace(SDL_SCANCODE_EQUALS, std::make_pair(KMOD_NONE, ley::Command::increaseVolume));
    mKeyBindings.emplace(SDL_SCANCODE_EQUALS, std::make_pair(KMOD_RSHIFT, ley::Command::increaseTransparency));
    
    mKeyBindings.emplace(SDL_SCANCODE_KP_PLUS, std::make_pair(KMOD_NONE, ley::Command::increaseVolume));

    mKeyBindings.emplace(SDL_SCANCODE_BACKSPACE, std::make_pair(KMOD_NONE, ley::Command::backspace));

    mKeyBindings.emplace(SDL_SCANCODE_TAB, std::make_pair(KMOD_NONE, ley::Command::tab));

    mKeyBindings.emplace(SDL_SCANCODE_C, std::make_pair(KMOD_NONE, ley::Command::debugclear));

    mKeyBindings.emplace(SDL_SCANCODE_D, std::make_pair(KMOD_RCTRL, ley::Command::restoredefault));
    mKeyBindings.emplace(SDL_SCANCODE_D, std::make_pair(KMOD_NONE, ley::Command::debugtexture));
    
    mKeyBindings.emplace(SDL_SCANCODE_F, std::make_pair(KMOD_NONE, ley::Command::debugfill));

    mKeyBindings.emplace(SDL_SCANCODE_I, std::make_pair(KMOD_NONE, ley::Command::debugnextlevel));

    mKeyBindings.emplace(SDL_SCANCODE_L, std::make_pair(KMOD_NONE, ley::Command::debugonlyline));

    mKeyBindings.emplace(SDL_SCANCODE_S, std::make_pair(KMOD_NONE, ley::Command::debugcolide));

    mKeyBindings.emplace(SDL_SCANCODE_U, std::make_pair(KMOD_NONE, ley::Command::debugprevlevel));
}

void ley::GameModel::loadButtonBindings() {
    
    mButtonBindings.insert({SDL_CONTROLLER_BUTTON_DPAD_DOWN, ley::Command::down});

    mButtonBindings.insert({SDL_CONTROLLER_BUTTON_DPAD_RIGHT, ley::Command::right});

    mButtonBindings.insert({SDL_CONTROLLER_BUTTON_DPAD_LEFT, ley::Command::left});

    mButtonBindings.insert({SDL_CONTROLLER_BUTTON_START, ley::Command::pause});

    mButtonBindings.insert({SDL_CONTROLLER_BUTTON_B, ley::Command::space});

    mButtonBindings.insert({SDL_CONTROLLER_BUTTON_Y, ley::Command::cclockwise});
    mButtonBindings.insert({SDL_CONTROLLER_BUTTON_DPAD_UP, ley::Command::cclockwise});

    mButtonBindings.insert({SDL_CONTROLLER_BUTTON_X, ley::Command::clockwise});

    mButtonBindings.insert({SDL_CONTROLLER_BUTTON_A, ley::Command::enter});

    mButtonBindings.insert({SDL_CONTROLLER_BUTTON_BACK, ley::Command::quit});

}

ley::KeyBindingsType* ley::GameModel::getKeyBindingsPtr() {
    return &mKeyBindings;
}

ley::PadBindingsType* ley::GameModel::getButtonBindingsPtr() {
    return &mButtonBindings;
}

std::string ley::GameModel::getKeyInputString(std::string seperator, ley::Command command, KeyBindingsType* bindings) {

    std::string output = "";
    for(const auto& kv : *bindings) {
        if (kv.second.second == command) {
            
            if(output != "") {
                //assume we already have a command and postpend the seperator
                output += seperator + " ";
            }

            // TODO can we do this without casting?
            output += (std::string)SDL_GetScancodeName((SDL_Scancode)kv.first);
        }
    }

    return output;
}

std::string ley::GameModel::getPadInputString(std::string seperator, ley::Command command, PadBindingsType* bindings) {

    std::string output = "";
    for(const auto& kv : *bindings) {
        if (kv.second == command) {
            
            if(output != "") {
                //assume we already have a command and postpend the seperator
                output += seperator + " ";
            }
            
            // TODO can we do this without casting?
            output += (std::string)SDL_GameControllerGetStringForButton((SDL_GameControllerButton)kv.first);

        }
    }

    return output;
}

void ley::GameModel::writeConfig() {
  std::ofstream myfile;
  myfile.open("mainconfig.csv");

  myfile << "language" << ',' << getLanguageModel()->getLanguage() << std::endl;
  myfile << "keydelay" << ',' << getKeyDelay() << std::endl;
  myfile << "keyrepeat" << ',' << getKeyRepeat() << std::endl;
  myfile << "guidegridon" << ',' << getGuideGridOn() << std::endl;
  myfile << "wallkickon" << ',' << getWallKickOn() << std::endl;
  
  myfile.close();
}

void ley::GameModel::readConfig() {
    
    std::string key;
    std::string value;

    std::ifstream inFile("mainconfig.csv");
    if (inFile.is_open())
    {
        std::string line;
        while( std::getline(inFile,line) )
        {
            std::stringstream ss(line);
            
            std::getline(ss,key,',');
            std::getline(ss,value,',');

            if(!value.empty() && !key.empty()) {

                if(key == "language") {
                    getLanguageModel()->setLanguage(value);
                }

                if(key == "keydelay") {
                    setKeyDelay(stoi(value));
                }

                if(key == "keyrepeat") {
                    setKeyRepeat(stoi(value));
                }

                if(key == "guidegridon") {
                    if(value == "cyan") {
                        setGuideGridOn("cyan");
                    }
                    if(value == "off") {
                        setGuideGridOn("off");
                    }
                    if(value == "red") {
                        setGuideGridOn("red");
                    }
                    if(value == "green") {
                        setGuideGridOn("green");
                    }
                    if(value == "yellow") {
                        setGuideGridOn("yellow");
                    }
                }    
                
                if(key == "wallkickon") {
                    if(value == "on") {
                        setWallKickOn("on");
                    }
                    else if(value == "off") {
                        setWallKickOn("off");
                    }

                }
            }
        }
    }
}

void ley::GameModel::setGuideGridOn(std::string inOn) {
    mGuideGridOn = inOn;
}

void ley::GameModel::setWallKickOn(std::string on) {
    mWallKickOn = on;
}

void ley::GameModel::readBlockData() {
    SDL_Log("ley::GameModel::readBlockData()");
    std::string key = "";
    std::string value = "";

    mBlockMapData.clear();

    std::ifstream inFile("blocks.csv");
    if (inFile.is_open())
    {
        std::string line;
        while( std::getline(inFile,line))
        {
            if (line.empty()) continue;
            std::stringstream ss(line);
            
            std::getline(ss,key,',');
            std::getline(ss,value,',');

            if(!value.empty() && !key.empty()) {
                // TODO sanatize values
                mBlockMapData.emplace(key,value);
            }
        }
    }

    inFile.close();

    addCanRotateToBlockData();
    logBlockData();
}

void ley::GameModel::addCanRotateToBlockData() {

    std::vector<std::string> blocksToCheck;
    blocksToCheck.push_back("a");
    blocksToCheck.push_back("b");
    blocksToCheck.push_back("c");
    blocksToCheck.push_back("d");
    blocksToCheck.push_back("e");
    blocksToCheck.push_back("f");
    blocksToCheck.push_back("g");

    for(std::string str : blocksToCheck) {
        
        for(int i = 0; i < BLOCK_SIZE; ++i) {
            
            if( mBlockMapData[str + "-" + std::to_string(0) + "-" + std::to_string(i)] != mBlockMapData[str + "-" + std::to_string(1) + "-" + std::to_string(i)]
             || mBlockMapData[str + "-" + std::to_string(0) + "-" + std::to_string(i)] != mBlockMapData[str + "-" + std::to_string(2) + "-" + std::to_string(i)]
             || mBlockMapData[str + "-" + std::to_string(0) + "-" + std::to_string(i)] != mBlockMapData[str + "-" + std::to_string(3) + "-" + std::to_string(i)]
             || mBlockMapData[str + "-" + std::to_string(0) + "-" + std::to_string(i)] != mBlockMapData[str + "-" + std::to_string(3) + "-" + std::to_string(i)]
            ) {

                mBlockMapData[str + "-*"] = "yes";
                i = BLOCK_SIZE; //were done with this for loop go to the next block.
            }
            else {
                mBlockMapData[str + "-*"] = "no";
            }
        }
    }
}

void ley::GameModel::logBlockData() {

    SDL_Log("Log Block Data:");
    for (const auto& pair : mBlockMapData) {
        
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}

ley::BlockFileDataMapType* ley::GameModel::getFileDataPtr() {
    return &mBlockMapData;
}

void ley::GameModel::initGame() {
    activeBlock = getRandomBlock();
    oldBlock = {activeBlock.getRect().x,activeBlock.getRect().y,activeBlock.getType(),1};
    nextBlock = getRandomBlock();
}