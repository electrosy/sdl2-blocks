/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A GameModel stores a representation of the game state.
Date: Feb/15/2020
*/
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <array>
#include <fstream>
#include <sstream>

#include "Audio.h"
#include "Block.h"
#include "Clock.h"
#include "Timer.h"
#include "Input.h"
#include "HighScores.h"
#include "../inc/Config.h"
#include "../inc/Board.h"
#include "../inc/LanguageModel.h"

namespace ley {

const Uint8 DROPCOOLDOWNMS_DEFAULT = 100;

enum class StateChange { //TODO this can probably be called State
    play,
    options,
    quitstate,
    highscores,
    gameover,
    credits,
    keyboardoptions,
    languageoptions,
    blockeditor,
    none}; //particular states that the game can be in

class GameModel {

private:
    bool mDebugOnlyLine = false;        //use only the line block for testing purposes
    bool mShowProgressBar = false;
    Board mBoard;
    Block mActiveBlock;
    Block mOldBlock;
    Block mNextBlock;
    bool mOverLayOn;
    bool mGameRunning;
    bool mActive;                       //not paused // TODO this should be called paused and the checks should be reversed
    bool mProgramRunning;               //if true then the program is still runing and has not been asked to exit yet
    bool mNewLevelToReport{true};       //true if the level has changed but has not been passed along yet, start with true for game init
    bool mDebugMode = false;            //allow debug commands
    double mCurrentSpeed;               //how many miliseconds until the block falls down
    int mNumLines;                      //number of lines the player has successfully completed. (Lines)
    int mNumLevel;                      //the current level we are on. A combination of lines. Aprox. 10 lines per level. (Level)
    long mScore;                        //the total score the this game (level*linesatonce)
    int mComboCount = 0;
    int mKeyDelay = ley::KEY_DELAY_TIME_DEFAULT;
    int mKeyRepeat = ley::KEY_REPEAT_TIME_DEFAULT;
    std::string mGuideGridOn = "purple";
    ley::Audio mAudioSystem;            //audio subsystem
    ley::StateChange mStateChange = ley::StateChange::none;
    KeyBindingsType mKeyBindings;       // Keyboard bindings
    PadBindingsType mButtonBindings;    // Gamepad bindings
    ley::HighScores mHighScores;
    bool mNewHighScore = false;
    int mPts_Line;
    ley::Config mConfig;
    ley::LanguageModel mLanguageModel;
    std::string mWallKickOn = "on";
    BlockFileDataMapType mBlockMapData;
    Uint8 mDropCoolDownMs = DROPCOOLDOWNMS_DEFAULT;
    int calcLevel();                    //Calculate current level based on number of lines completed
    void clearAndRecordLine(/*int, int*/ int lineNum); //clear the completed lines and keep track of the score
    void clearOldBlock();
    std::vector<char> checkForLines(char start);
    int firstLineAt(int);               //returns the first complete line from the bottom or -1 if there is no line
    bool processLines(int &numLines);   //returns true if any number of lines are removed.
    void shiftBoard(char, char);        //start line, number of lines
    void fillTop(char);                 //fill top of the board after the shift
    ley::Block getRandomBlock();
    ley::Block debugGetBlock();         //returns a specific block
    void updateSpeed();                 //check to see if the speed of the falldown block needs to change based on lines/score
    void onLine(int numLines, int level); //Handler when a line is completed
    void onDrop();
    void loadKeyBindings();             //keyboard
    void loadButtonBindings();          //gamepad
    void logBlockData();
    void addCanRotateToBlockData();     // we calculate if the block can rotate by checking to see if any orientation is different
    void initGame();                    // set up the blocks. clear and or initialize any other important values
    void readGamePadConfig(std::vector<ControllerButtonRow>* data);
    void readKeyboardConfig(std::vector<KeyBindingRow>* data);
    void writeGamePadConfig();
    void writeKeyboardConfig();
    SDL_Scancode mLastScancode;
    SDL_GameControllerButton mLastButtoncode;
    bool mWaitForKeydown = false;
    bool mKeyDownEvent = false;
    bool mWaitForButtonPress = false;
    bool mButtonPressEvent = false;

public:
    GameModel();
    ~GameModel();
    Board* getBoard();
    void debugResetActiveBlock();
    bool moveBlock(Command);            //returns true for false if block actually moved
    std::pair<bool, std::string> rotateBlock(bool);
    std::pair<bool, std::string> canRotate(bool); //false for counterclockwise and true for clockwise
    void overlayToggle();
    bool isOverlayOn();
    bool newBlock();
    int getScore();
    int getLevel();
    int getLines();
    void addToScore(long);
    bool isGameRunning();
    void setGameRunning(bool running);
    ley::Block getNextBlock();
    void resetGame();
    double speed() { return mCurrentSpeed; };
    void pauseGame(bool);
    bool isPaused();
    bool programRunning();              //is the program running?
    void stopProgram(bool);             //sets the program to exit by setting running to false.
    bool newLevel();                    //retruns true if there is a new level to report.
    ley::Audio* audio() { return &mAudioSystem; };
    ley::StateChange currentStateChange() { return mStateChange; }; //TODO this can probably be called gotoState
    void stateChange(ley::StateChange state) { mStateChange = state; };
    ley::HighScores* highScores() { return &mHighScores; };
    bool debugMode();                   //get current debugCommands flag
    void debugCommandsToggle();
    bool debugOnlyLine();
    void debugOnlyLineToggle();
    void debugNextLevel() {                   mNumLevel++; mNewLevelToReport=true; };
    void debugPrevLevel() { if(mNumLevel > 0){ mNumLevel--; } mNewLevelToReport=true; };
    void newHighScore(bool flag);
    bool newHighScore();
    void quickDrop();
    int comboCount() { return mComboCount;};
    Board* getNewBoard() { return &mBoard;};
    void resizeBoard(int width, int height);
    KeyBindingsType* getKeyBindingsPtr(); //keyboard
    PadBindingsType* getButtonBindingsPtr(); //gamepad
    static std::string getKeyInputString(std::string seperator, ley::Command command, KeyBindingsType* bindings);
    static std::string getPadInputString(std::string seperator, ley::Command command, PadBindingsType* bindings);
    ley::LanguageModel* getLanguageModel() { return &mLanguageModel; };
    void writeConfig();
    void readConfig();
    int getKeyDelay() {return mKeyDelay;};
    void setKeyDelay(int inKeyDelay) {mKeyDelay = inKeyDelay;};
    int getKeyRepeat() {return mKeyRepeat;};
    void setKeyRepeat(int inKeyRepeat) {mKeyRepeat = inKeyRepeat;};
    void readConfigOther();
    std::string getGuideGridOn() { return mGuideGridOn;};
    void setGuideGridOn(std::string inOn);
    bool rotateWithKick(bool r);
    ley::Block getActiveBlock() { return mActiveBlock; };
    std::string getWallKickOn() { return mWallKickOn; };
    void setWallKickOn(std::string on);
    BlockFileDataMapType* getFileDataPtr();
    void readBlockData();               // read in the block data from file
    Uint8 getDropCoolDown() { return mDropCoolDownMs; };
    void setDropCoolDown(Uint8 dropCoolDown) { mDropCoolDownMs = dropCoolDown; };
    SDL_Scancode getLastScancode() { return mLastScancode; };
    SDL_GameControllerButton getLastButton() { return mLastButtoncode; };
    void setLastScancode(SDL_Scancode sdlScancode) { mLastScancode = sdlScancode; };
    void setLastButton(SDL_GameControllerButton sdlbuttoncode) { mLastButtoncode = sdlbuttoncode; };
    bool waitForKeyDown(); //returns true when a new keydown event has been made but the message hasen't been picked up yet.
    bool waitForButtonPress();
    void setKeyDownEvent(bool in) { mKeyDownEvent = in; };
    void setButtonPressEvent(bool in) { mButtonPressEvent = in; };
    void setShowProgressBar(bool inShow) { mShowProgressBar = inShow; };
    bool getShowProgressBar() { return mShowProgressBar; };

};

}
#endif
