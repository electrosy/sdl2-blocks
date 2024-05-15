/* 
sdl2-blocks
Copyright (C) 2020-2021 Steven Philley

Purpose: Controlls the flow of the game and moves the active 
         block when its time to.
Date: Feb/15/2020
*/
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Video.h"
#include "GameModel.h"
#include "Clock.h"
#include "Block.h"
#include "Font.h"
#include "Timer.h"
#include "HighScores.h"
#include "inc/State/GameStateMachine.h"
#include "inc/State/CreditsState.h"
#include "inc/State/MenuState.h"
#include "inc/State/OptionMenuState.h"
#include "inc/State/HighScoresMenuState.h"
#include "inc/State/PlayState.h"
#include "inc/State/PauseState.h"
#include "inc/State/IntroState.h"
#include "inc/State/GameOverState.h"

namespace ley {

enum class InputWindow {
    goto_textBox,
    textBox,
    goto_game,
    game
};

class GameController {

private:
    ley::Video* mVideoSystem;
    SDL_Renderer* ren;
    ley::GameModel* gm;
    ley::GameStateMachine gameStateMachine;
    ley::Input mInputSystem;
    ley::Input mainInput; //create the input system object.

    //Textures
    SDL_Texture *blockBits2 = nullptr;

public:
    GameController(ley::Video* , ley::GameModel*);
    ~GameController();

    void processCommands(ley::Command command);
    void processStates(ley::Command command);
    void runGameLoop();
    void setHighScores(ley::HighScores* hs);
    void runCleanUp(); //run clean up after the game is completly over
    std::pair<int, int> centerRectInPx(SDL_Rect outer, SDL_Rect inner); // returns the x y for the inner rect to be in center of the outer rect
    void renderNextBlock();

    /* AUDIO */
    void fadeMusic(); //fade out the music
    void playMainMenu(); //play the main menu music
    void startPlayList();
    void playNext();
};

}
#endif


