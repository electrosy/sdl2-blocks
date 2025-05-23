/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/15/2020
*/
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>

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
numLevel(1),
gameRunning(true), 
currentSpeed(1000.0f), 
active(true), 
overlayOn(false),
score(0),
running(true),
mDebugOnlyLine(false)
{
    oldBlock.setH(activeBlock.getRect().h);
    oldBlock.setW(activeBlock.getRect().w);
      
    mHighScores.read();

//    loadKeyBindings();
    loadKeyBindings2();
    loadButtonBindings();

    readConfig();
    // TODO add the board size to the mainconfig
    readConfigOther();
    
    mLanguageModel.loadLanguage();
}

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
    if(!mBoard.canPut(activeBlock, ley::Command::up)) {
        return false;
    }
    else {
        mBoard.putBlock(activeBlock);
        return true;
    }
   
    return false;
}

bool ley::GameModel::rotateBlock(bool r) { // TODO, maybe ADD FEATURE - add control to Flip horz or vert.
    
    bool rotated = false;
    
    if(canRotate(r)) {
        activeBlock.rotate(r);        
        clearOldBlock();
        oldBlock.rotate(r);
        // new board
        mBoard.putBlock(activeBlock);
        rotated = true;
    }

    return rotated;
}

bool ley::GameModel::canRotate(bool r) {
    //rotate the block if the game is not paused
    bool canput = false;

    if(!isPaused() && activeBlock.rotate(r)) {
        canput = mBoard.canPut(activeBlock, ley::Command::up);
        activeBlock.rotate(!r); //rotate it back, because this 
                                //function is simply a test only
    }
    
    return canput;
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
            if (mBoard.canPut(activeBlock, Command::down)) {
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
            if (mBoard.canPut(activeBlock, Command::left)) {
                activeBlock.moveLeft();
                clearOldBlock();
                oldBlock.moveLeft();
                moved = true;
            }
        break;

        case Command::right :
            //new board
            if (mBoard.canPut(activeBlock, Command::right)) {
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
    while(ley::GameModel::moveBlock(ley::Command::down))
    {

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

/*

void ley::GameModel::loadKeyBindings() {

    mKeyBindings.insert({SDL_SCANCODE_F12, ley::Command::debugkeystoggle});

    mKeyBindings.insert({SDL_SCANCODE_LEFT, ley::Command::left});

    mKeyBindings.insert({SDL_SCANCODE_RIGHT, ley::Command::right});

    mKeyBindings.insert({SDL_SCANCODE_DOWN, ley::Command::down});

    mKeyBindings.insert({SDL_SCANCODE_UP, ley::Command::cclockwise});
    mKeyBindings.insert({SDL_SCANCODE_E, ley::Command::cclockwise});
    
    mKeyBindings.insert({SDL_SCANCODE_R, ley::Command::clockwise});

    mKeyBindings.insert({SDL_SCANCODE_Q, ley::Command::quit});
    mKeyBindings.insert({SDL_SCANCODE_ESCAPE, ley::Command::quit});

    mKeyBindings.insert({SDL_SCANCODE_RETURN, ley::Command::enter});

    mKeyBindings.insert({SDL_SCANCODE_N, ley::Command::nextSong});

    mKeyBindings.insert({SDL_SCANCODE_B, ley::Command::previousSong});

    mKeyBindings.insert({SDL_SCANCODE_SPACE, ley::Command::space});

    mKeyBindings.insert({SDL_SCANCODE_P, ley::Command::pause});
    mKeyBindings.insert({SDL_SCANCODE_SLASH, ley::Command::pause});

    mKeyBindings.insert({SDL_SCANCODE_MINUS, ley::Command::decreaseVolume});
    mKeyBindings.insert({SDL_SCANCODE_KP_MINUS, ley::Command::decreaseVolume});

    mKeyBindings.insert({SDL_SCANCODE_EQUALS, ley::Command::increaseVolume});
    mKeyBindings.insert({SDL_SCANCODE_KP_PLUS, ley::Command::increaseVolume});

    mKeyBindings.insert({SDL_SCANCODE_BACKSPACE, ley::Command::backspace});

    mKeyBindings.insert({SDL_SCANCODE_TAB, ley::Command::tab});

    mKeyBindings.insert({SDL_SCANCODE_C, ley::Command::debugclear});

    mKeyBindings.insert({SDL_SCANCODE_F, ley::Command::debugfill});

    mKeyBindings.insert({SDL_SCANCODE_L, ley::Command::debugonlyline});

    mKeyBindings.insert({SDL_SCANCODE_D, ley::Command::debugtexture});

    mKeyBindings.insert({SDL_SCANCODE_S, ley::Command::debugcolide});

    mKeyBindings.insert({SDL_SCANCODE_GRAVE, ley::Command::console});

    mKeyBindings.insert({SDL_SCANCODE_I, ley::Command::debugnextlevel});

    mKeyBindings.insert({SDL_SCANCODE_U, ley::Command::debugprevlevel});
}

*/

void ley::GameModel::loadKeyBindings2() {

    mKeyBindings2.emplace(SDL_SCANCODE_F12, std::make_pair(KMOD_NONE, ley::Command::debugkeystoggle));

    mKeyBindings2.emplace(SDL_SCANCODE_LEFT, std::make_pair(KMOD_NONE, ley::Command::left));

    mKeyBindings2.emplace(SDL_SCANCODE_RIGHT, std::make_pair(KMOD_NONE, ley::Command::right));

    mKeyBindings2.emplace(SDL_SCANCODE_DOWN, std::make_pair(KMOD_NONE, ley::Command::down));

    mKeyBindings2.emplace(SDL_SCANCODE_UP, std::make_pair(KMOD_NONE, ley::Command::cclockwise));
    mKeyBindings2.emplace(SDL_SCANCODE_E, std::make_pair(KMOD_NONE, ley::Command::cclockwise));
    
    mKeyBindings2.emplace(SDL_SCANCODE_R, std::make_pair(KMOD_NONE, ley::Command::clockwise));

    mKeyBindings2.emplace(SDL_SCANCODE_Q, std::make_pair(KMOD_NONE, ley::Command::quit));
    mKeyBindings2.emplace(SDL_SCANCODE_ESCAPE, std::make_pair(KMOD_NONE, ley::Command::quit));

    mKeyBindings2.emplace(SDL_SCANCODE_RETURN, std::make_pair(KMOD_NONE, ley::Command::enter));
    
    mKeyBindings2.emplace(SDL_SCANCODE_RETURN, std::make_pair(KMOD_ALT, ley::Command::fullscreen));

    mKeyBindings2.emplace(SDL_SCANCODE_N, std::make_pair(KMOD_NONE, ley::Command::nextSong));

    mKeyBindings2.emplace(SDL_SCANCODE_B, std::make_pair(KMOD_NONE, ley::Command::previousSong));

    mKeyBindings2.emplace(SDL_SCANCODE_SPACE, std::make_pair(KMOD_NONE, ley::Command::space));

    mKeyBindings2.emplace(SDL_SCANCODE_P, std::make_pair(KMOD_NONE, ley::Command::pause));
    mKeyBindings2.emplace(SDL_SCANCODE_SLASH, std::make_pair(KMOD_NONE, ley::Command::pause));

    
    mKeyBindings2.emplace(SDL_SCANCODE_MINUS, std::make_pair(KMOD_NONE, ley::Command::decreaseVolume));
    mKeyBindings2.emplace(SDL_SCANCODE_MINUS, std::make_pair(KMOD_RSHIFT, ley::Command::decreaseTransparency));
    
    mKeyBindings2.emplace(SDL_SCANCODE_KP_MINUS, std::make_pair(KMOD_NONE, ley::Command::decreaseVolume));

    mKeyBindings2.emplace(SDL_SCANCODE_EQUALS, std::make_pair(KMOD_NONE, ley::Command::increaseVolume));
    mKeyBindings2.emplace(SDL_SCANCODE_EQUALS, std::make_pair(KMOD_RSHIFT, ley::Command::increaseTransparency));
    
    mKeyBindings2.emplace(SDL_SCANCODE_KP_PLUS, std::make_pair(KMOD_NONE, ley::Command::increaseVolume));

    mKeyBindings2.emplace(SDL_SCANCODE_BACKSPACE, std::make_pair(KMOD_NONE, ley::Command::backspace));

    mKeyBindings2.emplace(SDL_SCANCODE_TAB, std::make_pair(KMOD_NONE, ley::Command::tab));

    mKeyBindings2.emplace(SDL_SCANCODE_C, std::make_pair(KMOD_NONE, ley::Command::debugclear));

    mKeyBindings2.emplace(SDL_SCANCODE_F, std::make_pair(KMOD_NONE, ley::Command::debugfill));

    mKeyBindings2.emplace(SDL_SCANCODE_L, std::make_pair(KMOD_NONE, ley::Command::debugonlyline));

    mKeyBindings2.emplace(SDL_SCANCODE_D, std::make_pair(KMOD_NONE, ley::Command::debugtexture));

    mKeyBindings2.emplace(SDL_SCANCODE_S, std::make_pair(KMOD_NONE, ley::Command::debugcolide));

    mKeyBindings2.emplace(SDL_SCANCODE_GRAVE, std::make_pair(KMOD_NONE, ley::Command::console));

    mKeyBindings2.emplace(SDL_SCANCODE_I, std::make_pair(KMOD_NONE, ley::Command::debugnextlevel));

    mKeyBindings2.emplace(SDL_SCANCODE_U, std::make_pair(KMOD_NONE, ley::Command::debugprevlevel));
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

std::map<Uint8, ley::Command>* ley::GameModel::getKeyBindingsPtr() {
    return &mKeyBindings;
}

ley::BindingsType* ley::GameModel::getKeyBindingsPtr2() {
    return &mKeyBindings2;
}

std::map<Uint8, ley::Command>* ley::GameModel::getButtonBindingsPtr() {
    return &mButtonBindings;
}

std::string ley::GameModel::getInputsString(std::string seperator, ley::Command command, std::map<Uint8, ley::Command>* bindings, bool gamepad) {

    std::string output = "";
    for(const auto& kv : *bindings) {
        if (kv.second == command) {
            if(output != "") {
                //assume we already have a command and postpend the seperator
                output += seperator + " ";
            }
            if(gamepad) {
                output += (std::string)SDL_GameControllerGetStringForButton((SDL_GameControllerButton)kv.first);
                
            }
            else {
                output += (std::string)SDL_GetScancodeName((SDL_Scancode)kv.first);
            }
        }
    }

    return output;
}

// TODO getInputsString needs to be removed once the game pad logic has been adjusted 
//      for the new BindingsType then rename getInputsString2 to getInputsString

std::string ley::GameModel::getInputsString2(std::string seperator, ley::Command command, BindingsType* bindings, bool gamepad) {

    std::string output = "";
    for(const auto& kv : *bindings) {
        if (kv.second.second == command) {
            
            if(output != "") {
                //assume we already have a command and postpend the seperator
                output += seperator + " ";
            }
            if(gamepad) {
                output += (std::string)SDL_GameControllerGetStringForButton((SDL_GameControllerButton)kv.first);
                
            }
            else {
                output += (std::string)SDL_GetScancodeName((SDL_Scancode)kv.first);
            }
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
            }
        }
    }

    
}