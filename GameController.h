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
#include "./inc/GameModel.h"
#include "./inc/Clock.h"
#include "./inc/Block.h"
#include "./inc/Font.h"
#include "./inc/Timer.h"
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
#include "inc/State/KeyboardOptionsState.h"
#include "inc/State/LanguageOptionsState.h"
#include "inc/State/BlockEditorState.h"

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
    SDL_Renderer* mRen;
    ley::GameModel* mGm;
    ley::GameStateMachine mGameStateMachine;
    ley::Input mInputSystem; //create the input system object.
    std::queue<ley::Command> mCommandQueue;

    //Textures
    SDL_Texture *mBlockBits2Ptr = nullptr;

public:
    GameController(ley::Video* , ley::GameModel*);
    ~GameController();

    void processCommands(ley::Command command);
    void processStates(ley::Command command);
    void runGameLoop();
    void setHighScores(ley::HighScores* hs);
    void runCleanUp(); //run clean up after the game is completly over
    SDL_Point centerRectInPx(SDL_Rect outer, SDL_Rect inner); // returns the x y for the inner rect to be in center of the outer rect
    void renderNextBlock();

    /* AUDIO */
    void fadeMusic(); //fade out the music
    void playMainMenu(); //play the main menu music
    void startPlayList();
    void playNext();
};

}
#endif



