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
#include "inc/Config.h"
#include "inc/Board.h"
#include "inc/LanguageModel.h"

namespace ley {

enum class StateChange { //TODO this can probably be called State
    play,
    options,
    quitstate,
    highscores,
    gameover,
    credits,
    keyboardoptions,
    languageoptions,
    none}; //particular states that the game can be in

// TODO what is this PTS_LINE used for?
//const auto PTS_LINE = BOARDSIZE_WIDTH * (PTS_DROP * 2); // x Level

class GameModel {

private:
    bool mDebugOnlyLine = false; //use only the line block for testing purposes
    Board mBoard;
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
    int mComboCount = 0;
    void clearOldBlock();
    int mKeyDelay = ley::KEY_DELAY_TIME_DEFAULT;
    int mKeyRepeat = ley::KEY_REPEAT_TIME_DEFAULT;
    std::vector<char> checkForLines(char start);
    int firstLineAt(int); //returns the first complete line from the bottom or -1 if there is no line.
    bool processLines(int &numLines); //returns true if any number of lines are removed.
    void clearAndRecordLine(/*int, int*/ int lineNum); //clear the completed lines and keep track of the score.
    void shiftBoard(char, char); //start line, number of lines
    void fillTop(char); //fill top of the board after the shift
    ley::Block getRandomBlock();
    ley::Block debugGetBlock();
    void updateSpeed(); //check to see if the speed of the falldown block needs to change based on lines/score
    bool running; //if true then the program is still runing and has not been asked to exit yet.
    int calcLevel(); //Calculate current level based on number of lines completed
    bool mDebugMode = false; //allow debug commands
    ley::Audio audSystem; //audio subsystem.
    ley::StateChange mStateChange = ley::StateChange::none;
    std::map<Uint8, ley::Command> mKeyBindings; // keyboard bindings
    KeyBindingsType mKeyBindings2;
//    std::map<Uint8, ley::Command> mButtonBindings; // gamepad bindings
    PadBindingsType mButtonBindings2; // gamepad bindings
    ley::HighScores mHighScores;
    bool mNewHighScore = false;
    int mPts_Line;
    ley::Config mConfig;
    void onLine(int numLines, int level); //Handler when a line is completed.
    void onDrop();
    void loadKeyBindings2(); //keyboard
//    void loadButtonBindings(); //gamepad
    void loadButtonBindings2(); //gamepad
    ley::LanguageModel mLanguageModel;    

public:
    GameModel();
    ~GameModel();
    Board* getBoard();
    void debugResetActiveBlock();
    bool moveBlock(Command); //returns true for false if block actually moved
    bool rotateBlock(bool);
    bool canRotate(bool); //false for counterclockwise and true for clockwise
    void overlayToggle();
    bool isOverlayOn();
    bool newBlock();
    void setBlock();
    int getScore();
    int getLevel();
    int getLines();
    void addToScore(long);
    bool isGameRunning();
    void setGameRunning(bool running);
    ley::Block getNextBlock();
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
    bool debugMode(); //get current debugCommands flag
    void debugCommandsToggle();
    bool debugOnlyLine();
    void debugOnlyLineToggle();
    void debugNextLevel() {                   numLevel++; newLevelToReport=true; };
    void debugPrevLevel() { if(numLevel > 0){ numLevel--; } newLevelToReport=true; };
    void newHighScore(bool flag);
    bool newHighScore();
    void quickDrop();
    int comboCount() { return mComboCount;};
    Board* getNewBoard() { return &mBoard;};
    void resizeBoard(int width, int height);
    KeyBindingsType* getKeyBindingsPtr2(); //keyboard
//    std::map<Uint8, ley::Command>* getButtonBindingsPtr(); //gamepad
    PadBindingsType* getButtonBindingsPtr2(); //gamepad
    static std::string getInputsString(std::string seperator, ley::Command command, std::map<Uint8, ley::Command>* bindings, bool gamepad);
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
};

}
#endif
