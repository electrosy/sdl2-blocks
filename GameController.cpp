/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/18/2020
*/
#include <SDL2/SDL.h>
#include "GameController.h"
#include "Textures.h"
#include "Block.h"
#include <map>

typedef ley::Textures TextureManager;


/* RAII */
ley::GameController::GameController(ley::Video * v, ley::GameModel *g)
: 
mVideoSystem(v),
mRen(mVideoSystem->getRenderer()),
mGm(g) {

    mGm->audio()->playIntro();
}

ley::GameController::~GameController() {
}
/* Accessors */

/* Functions */
void ley::GameController::runGameLoop() {

    mGameStateMachine.pushState(new ley::IntroState(mVideoSystem, mGm));

    bool fs = mVideoSystem->fullScreen();
    SDL_Log("Starting Game loop!");

    while(mGm->programRunning()) {
        mVideoSystem->frameStart(); //Need to call this at the begenning of the frame, for the frame delay.

        /**** RENDER ****/
        //Only render the game board if we are in PLAY, PAUSE or GAMEOVER.
        if(mGameStateMachine.getStateId() == "PLAY" 
            || mGameStateMachine.getStateId() == "PAUSE"
            || mGameStateMachine.getStateId() == "GAMEOVER") {
            
            mVideoSystem->render();
            renderNextBlock();
            //new board
            mGm->getNewBoard()->render(mRen, false);
        }
        mGameStateMachine.render();
        mVideoSystem->renderTopLayer();
        
        /**** PRESENT ****/
        mVideoSystem->present(); // output to the video system.
        
        /**** GET INPUT ****/
        //pollEvents updates running and full screen flags
        mInputSystem.pollEvents(fs, mGm->getButtonBindingsPtr(), mGm->getKeyBindingsPtr(), &mCommandQueue, mGameStateMachine.activeUIElement(),
            [this](ley::Command c) {mGameStateMachine.activeUIElement()->onKeyDown(c == ley::Command::backspace ? ley::Character::backspace : ley::Character::none);});

        ley::Command command = ley::Command::none;
        if(!mCommandQueue.empty()) {
            command = mCommandQueue.front();
            mCommandQueue.pop();
        }

        /**** INPUT PROCESSING ****/
        if(fs != mVideoSystem->fullScreen()) {
            mVideoSystem->setFullScreen(fs);
        }

        processCommands(command);
        mGameStateMachine.update(command);
        processStates(command);
        
        /**** CLEAR ****/
        mVideoSystem->clear(); //clear the backbuffer
        
        /**** LOCK FRAME RATE ****/
        mVideoSystem->frameDelay();
    }

    mGameStateMachine.popState(); //Remove the mainmenu state.

    /**** CLEAN UP ****/
    runCleanUp();
}
void ley::GameController::processCommands(ley::Command command) {
    if(command == ley::Command::nextSong) {
        playNext();
    }

    if(command == ley::Command::decreaseVolume) {
        mGm->audio()->decreaseVolume();
    }

    if(command == ley::Command::increaseVolume) {
        mGm->audio()->increaseVolume();
    }

    /**** DEBUG INPUT PROCESSING ****/
    if(command == ley::Command::debugkeystoggle) {
        mGm->debugCommandsToggle();
    }

    //allow debug commands to be used
    if(mGm->debugCommands()) {
        if(command == ley::Command::console) {
            mGm->overlayToggle();
        }

        if(command == ley::Command::debugclear) {
            //new board
            mGm->getNewBoard()->clear();
            mGm->debugResetActiveBlock();
        }

        if(command == ley::Command::debugfill) {
            //new board
            mGm->getNewBoard()->debugFill();
        }

        if(command == ley::Command::debugnextlevel) {
            mGm->debugNextLevel();
        }

        if(command == ley::Command::debugprevlevel) {
            mGm->debugPrevLevel();
        }

        if(command == ley::Command::debugonlyline) {
            mGm->debugOnlyLineToggle();
        }
    }
}
void ley::GameController::processStates(ley::Command inCommand) {
    //Quit the introstate and goto the menu state.
    if((inCommand == ley::Command::enter || mGameStateMachine.isStateDone())
        && mGameStateMachine.getStateId() == "INTRO") {
        mGameStateMachine.changeState(new ley::MenuState(mVideoSystem, mGm));
    }

    //Don't quit the state for pause this way.
    if((inCommand == ley::Command::quit || mGm->currentStateChange() == ley::StateChange::quitstate)
        && !(mGameStateMachine.getStateId() == "PAUSE")
        && !(mGameStateMachine.getStateId() == "INTRO")) {

        if(mGameStateMachine.getStateId() == "GAMEOVER" && !mGm->isGameRunning()) {
            mGameStateMachine.popState(); //Assume we have to exit two states if we are in game over. assume STACK=MENU|PLAY|GAMEOVER
        }

        mGameStateMachine.popState();
        mGm->stateChange(ley::StateChange::none);
    }

    if(mGm->currentStateChange() == ley::StateChange::play) {
        //add the new state
        mGm->audio()->fadeOutMusic();
        mGameStateMachine.pushState(new ley::PlayState(mVideoSystem, mGm));
        //remove the statechange flag
        mGm->stateChange(ley::StateChange::none);
    }

    if(mGm->currentStateChange() == ley::StateChange::highscores) {
        //add the new state
        mGameStateMachine.pushState(new ley::HighScoresMenuState(mVideoSystem, mGm));
        //remove the statechange flag
        mGm->stateChange(ley::StateChange::none);
    }

    if(mGm->currentStateChange() == ley::StateChange::options) {
        //Goto the options menu state
        mGameStateMachine.pushState(new ley::OptionMenuState(mVideoSystem, mGm));
        //clear the statechange flag
        mGm->stateChange(ley::StateChange::none);
    }

    if(mGm->currentStateChange() == ley::StateChange::keyboardoptions) {
        //Goto the keyboardoptions menu state
        mGameStateMachine.pushState(new ley::KeyboardOptionsState(mVideoSystem, mGm));
        //clear the statechange flag
        mGm->stateChange(ley::StateChange::none);
    }

    if(mGm->currentStateChange() == ley::StateChange::credits) {
        //Goto the options menu state
        mGameStateMachine.pushState(new ley::CreditsState(mVideoSystem, mGm));
        //clear the statechange flag
        mGm->stateChange(ley::StateChange::none);
    }

    //If the game stops running and we are in the play state then go to the game over state.
    if(!mGm->isGameRunning() && mGameStateMachine.getStateId() == "PLAY") {
      //  setHighScores(gm->highScores());

        // if new high score
        if(mGm->highScores()->isNewHigh(mGm->getScore()) > 0) {
            mGm->newHighScore(true);
        }
        
        mGameStateMachine.pushState(new ley::GameOverState(mVideoSystem, mGm));

        if(mGm->newHighScore()) {
            mGameStateMachine.pushState(new ley::HighScoresMenuState(mVideoSystem, mGm));
        }
        
        mGm->stateChange(ley::StateChange::none);
    }

    //Only allow paused/unpause if we are in the play or pause states
    if(inCommand == ley::Command::pause && (mGameStateMachine.getStateId() == "PLAY" || mGameStateMachine.getStateId() == "PAUSE")) {
        //Only allow pause if we are in the playstate.
        if(mGm->isPaused()) {
            SDL_Log("Game Paused!");
            mGm->audio()->playSfx(ley::sfx::pause); //TODO sfx can probably go in the pause state
            mGameStateMachine.pushState(new ley::PauseState(mVideoSystem, mGm));
        }

        if(!mGm->isPaused()) {
            SDL_Log("Game Resumed");    
            mGm->audio()->playSfx(ley::sfx::unpause); //TODO sfx can probably go in the pause state
            mGameStateMachine.popState();
        }
    }
}
void ley::GameController::setHighScores(ley::HighScores* hs) {
    //push on the new high score
    hs->setHighScore(mGm->getScore(), "Steve", mGm->getLevel(), mGm->getLines());
}

void ley::GameController::runCleanUp() {
    mGm->resetGame();
    fadeMusic();
}
std::pair<int, int> ley::GameController::centerRectInPx(SDL_Rect outer, SDL_Rect inner) {
    
    int x = 0;
    int y = 0;

    x = (outer.w/2) - (inner.w/2);
    y = (outer.h/2) - (inner.h/2);
    
    return std::make_pair(x,y);
}
void ley::GameController::renderNextBlock() {

    int w = BLOCKSIZE_PX, h = BLOCKSIZE_PX; //SDL_QueryTexture(t, NULL, NULL, &w, &h);

    SDL_Rect start_rect;
    start_rect.x = 0;
    start_rect.y = 0;
    start_rect.h = h;
    start_rect.w = w;

    ley::Block nextBlock = mGm->getNextBlock();

    std::pair<int, int> pos = centerRectInPx({mGm->getBoard()->nextBoxPosXPx(), NEXTBOX_POS_Y_PX, NEXTBOX_SIZE_PX, NEXTBOX_SIZE_PX},
                                            {nextBlock.getRect().x, nextBlock.getRect().y, nextBlock.width()*BLOCKSIZE_PX, nextBlock.height()*BLOCKSIZE_PX});

    SDL_Rect next_dest_rect;
    next_dest_rect.x = mGm->getBoard()->nextBoxPosXPx() + pos.first;
    next_dest_rect.y = NEXTBOX_POS_Y_PX + pos.second;
    next_dest_rect.h = h;
    next_dest_rect.w = w;
    
    for (auto row : nextBlock.getBlockParts())
    {
        for (auto column : row)
        {
            if (column != BlockTexCode::O)
            {
                //TODO - all rendering should be done in the View(Video.cpp)
                SDL_RenderCopy(mRen, TextureManager::Instance()->getTexture(TEXCODE_CHAR.at(column)), 
                    &start_rect, &next_dest_rect);
            }
            next_dest_rect.x = next_dest_rect.x + w;
        }
        next_dest_rect.y = next_dest_rect.y + h;
        next_dest_rect.x = mGm->getBoard()->nextBoxPosXPx() + pos.first;
    }
}

void ley::GameController::fadeMusic() { 
    mGm->audio()->fadeOutMusic();
}

void ley::GameController::playMainMenu() {
    mGm->audio()->playMainMenu();
}

void ley::GameController::startPlayList() {
    mGm->audio()->playPlaylist();
}

void ley::GameController::playNext() {
    mGm->audio()->playNext();
}
