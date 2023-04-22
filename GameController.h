/* 
sdl2-blocks
Copyright (C) 2020-2021 Steven Philley

Purpose: Controlls the flow of the game and moves the active 
         block when its time to.
Date: Feb/15/2020
*/
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Audio.h"
#include "Video.h"
#include "GameModel.h"
#include "Clock.h"
#include "Block.h"
#include "Font.h"
#include "Timer.h"
#include "Input.h"
#include "HighScores.h"
#include "inc/State/GameStateMachine.h"
#include "inc/State/MenuState.h"
#include "inc/State/PlayState.h"
#include "inc/State/IntroState.h"

namespace ley {

class GameController {

private:
    ley::Video* mVideoSystem;
    SDL_Renderer* ren;
    ley::GameModel* gm;
    ley::GameStateMachine gameStateMachine;
    ley::Input mInputSystem;
    
    int blockFallSpeed = 1000; //how many milisecond to make the block fall

    ley::Input mainInput; //create the input system object.

    //Textures
    SDL_Texture *blockBits2 = nullptr;

    //Timers
    ley::Timer fallTimer; //Time to force the blockdown    

    ley::Audio audSystem; //audio subsystem.

public:
    GameController(ley::Video* , ley::GameModel*);
    ~GameController();

    void runGameLoop(bool autoRestart, ley::HighScores &hs); // TODO temporary pass through params for the timers.
    void runGameOver(ley::HighScores &hs, bool fs);
    void runCleanUp(); //run clean up after the game is completly over
    ley::Timer* getFallTimer(); //TODO temporary accessor for the timer
    
    void renderBoard(); //renders the board to the video.
    void runIntro(std::string t, SDL_Rect r, double fpsDelay); //run intro screens
    void runIntros(); //run both intro screens
    void setState(int statenum);

    /* AUDIO */
    void fadeMusic(); //fade out the music
    void playMainMenu(); //play the main menu music
    void startPlayList();
    void playNext();
};

}
#endif
