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

typedef ley::Textures TextureManager;

const auto NEXTBLOCK_OFFSET_X_PX = 360;
const auto NEXTBLOCK_OFFSET_Y_PX = 10;

/* RAII */
ley::GameController::GameController(ley::Video * v, ley::GameModel *g)
: 
mVideoSystem(v),
ren(mVideoSystem->getRenderer()),
gm(g) {

    gm->audio()->playIntro();
}

ley::GameController::~GameController() {
}
/* Accessors */

/* Functions */
void ley::GameController::runGameLoop() {

    gameStateMachine.pushState(new ley::IntroState(mVideoSystem, gm));

    bool fs = mVideoSystem->fullScreen();
    SDL_Log("Starting Game loop!");

    while(gm->programRunning()) {
        /**** RENDER ****/
        //Only render the game board if we are in PLAY, PAUSE or GAMEOVER.
        if(gameStateMachine.getStateId() == "PLAY" 
            || gameStateMachine.getStateId() == "PAUSE"
            || gameStateMachine.getStateId() == "GAMEOVER") {
            
            mVideoSystem->render();
            renderBoard();
        }
        gameStateMachine.render();
        
        /**** PRESENT ****/
        mVideoSystem->present(); // output to the video system.
        
        /**** GET INPUT ****/
        //pollEvents updates running and full screen flags
        ley::Command command = mainInput.pollEvents(fs);
        
        /**** INPUT PROCESSING ****/
        if(command == ley::Command::nextSong) {
            playNext();
        }

        if(command == ley::Command::debugnextlevel) {
            gm->debugNextLevel();
        }

        if(command == ley::Command::debugprevlevel) {
            gm->debugPrevLevel();
        }

        if(fs != mVideoSystem->fullScreen()) {
            mVideoSystem->setFullScreen(fs);
        }

        if(command == ley::Command::decreaseVolume) {
            gm->audio()->decreaseVolume();
        }

        if(command == ley::Command::increaseVolume) {
            gm->audio()->increaseVolume();
        }

        gameStateMachine.update(command);

        //Quit the introstate and goto the menu state.
        if((command == ley::Command::enter || gameStateMachine.isStateDone())
            && gameStateMachine.getStateId() == "INTRO") {
            gameStateMachine.changeState(new ley::MenuState(mVideoSystem, gm));
        }

        //Don't quit the state for pause this way.
        if((command == ley::Command::quit || gm->currentStateChange() == ley::StateChange::quitstate)
            && !(gameStateMachine.getStateId() == "PAUSE")
            && !(gameStateMachine.getStateId() == "INTRO")) {

            if(gameStateMachine.getStateId() == "GAMEOVER" && !gm->isGameRunning()) {
                gameStateMachine.popState(); //Assume we have to exit two states if we are in game over. assume STACK=MENU|PLAY|GAMEOVER
            }

            gameStateMachine.popState();
            gm->stateChange(ley::StateChange::none);
        }

        if(gm->currentStateChange() == ley::StateChange::play) {
            //add the new state
            gm->audio()->fadeOutMusic();
            gameStateMachine.pushState(new ley::PlayState(mVideoSystem, gm));
            //remove the statechange flag
            gm->stateChange(ley::StateChange::none);
        }

        if(gm->currentStateChange() == ley::StateChange::highscores) {
            //add the new state
            gameStateMachine.pushState(new ley::HighScoresMenuState(mVideoSystem, gm));
            //remove the statechange flag
            gm->stateChange(ley::StateChange::none);
        }

        if(gm->currentStateChange() == ley::StateChange::options) {
            //Goto the options menu state
            gameStateMachine.pushState(new ley::OptionMenuState(mVideoSystem, gm));
            //clear the statechange flag
            gm->stateChange(ley::StateChange::none);
        }

        if(gm->currentStateChange() == ley::StateChange::credits) {
            //Goto the options menu state
            gameStateMachine.pushState(new ley::CreditsState(mVideoSystem, gm));
            //clear the statechange flag
            gm->stateChange(ley::StateChange::none);
        }

        //If the game stops running and we are in the play state then go to the game over state.
        if(!gm->isGameRunning() && gameStateMachine.getStateId() == "PLAY") {
            setHighScores(gm->highScores());
            gameStateMachine.pushState(new ley::GameOverState(mVideoSystem, gm));
            gm->stateChange(ley::StateChange::none);
        }

        //Only allow paused/unpause if we are in the play or pause states
        if(command == ley::Command::pause && (gameStateMachine.getStateId() == "PLAY" || gameStateMachine.getStateId() == "PAUSE")) {
            //Only allow pause if we are in the playstate.
            if(gm->isPaused()) {
                SDL_Log("Game Paused!");
                gm->audio()->playSfx(ley::sfx::pause); //TODO sfx can probably go in the pause state
                gameStateMachine.pushState(new ley::PauseState(mVideoSystem, gm));
            }

            if(!gm->isPaused()) {
                SDL_Log("Game Resumed");    
                gm->audio()->playSfx(ley::sfx::unpause); //TODO sfx can probably go in the pause state
                gameStateMachine.popState();
            }
        }
 
        /**** CLEAR ****/
        mVideoSystem->clear(); //clear the backbuffer
        
        /**** LOCK FRAME RATE ****/
        mVideoSystem->frameDelay();
    }

    gameStateMachine.popState(); //Remove the mainmenu state.

    /**** CLEAN UP ****/
    runCleanUp();
}
void ley::GameController::setHighScores(ley::HighScores* hs) {
    //push on the new high score
    hs->push(gm->getScore(), "Steve", gm->getLevel(), gm->getLines());
    hs->write();
    hs->setClean(false);
}

void ley::GameController::runCleanUp() {
    gm->resetGame();
    fadeMusic();
}

//TODO the board should be rendered in the state machine like everything else.
void ley::GameController::renderBoard(/*SDL_Texture* t*/) {
    //get width and height of the texture // TODO this should be dynamic based on image passed in
    int w = 30, h = 30; //SDL_QueryTexture(t, NULL, NULL, &w, &h);

    SDL_Rect start_rect;
    start_rect.x = 0;
    start_rect.y = 0;
    start_rect.h = h;
    start_rect.w = w;

    SDL_Rect dest_rect;
    dest_rect.x = START_X_OFFSET_PX;
    dest_rect.y = BOARD_OFFSET_PX;
    dest_rect.h = h;
    dest_rect.w = w;

    //TODO - Loop 1 and Loop 2 can probably be rafactored together.
    // Output the nextBlock from the game model
    SDL_Rect next_dest_rect;
    next_dest_rect.x = NEXTBLOCK_OFFSET_X_PX;
    next_dest_rect.y = 40 + NEXTBLOCK_OFFSET_Y_PX;
    next_dest_rect.h = h;
    next_dest_rect.w = w;
    ley::Block nextBlock = gm->getNextBlock();
    for (auto row : nextBlock.getBlockParts())
    {
        for (auto column : row)
        {
            if (column != BlockTexCode::O)
            {
                //TODO - all rendering should be done in the View(Video.cpp)
                SDL_RenderCopy(ren, TextureManager::Instance()->getTexture(TEXCODE_CHAR.at(column)), 
                    &start_rect, &next_dest_rect);
            }
            next_dest_rect.x = next_dest_rect.x + w;
        }
        next_dest_rect.y = next_dest_rect.y + h;
        next_dest_rect.x = NEXTBLOCK_OFFSET_X_PX;
    }

    // TODO loop2 refactor with loop 1 above
    // Loop through the game model and output a representation to the video screen.
    SDL_Texture *blockBits2 = nullptr;
    for (auto row : *gm->getBoard()) {
        for (auto column : row) {
            if (column.first != BlockTexCode::O) { 
            // TODO Change BlockTexCode::O to BlockTexCode::d to reverse textures(clear blocks)
                blockBits2 = TextureManager::Instance()->getTexture(TEXCODE_CHAR.at(column.first));

                //TODO - all rendering should be done in the View(Video.cpp)
                SDL_RenderCopy(ren, blockBits2, &start_rect, &dest_rect);
            }
            dest_rect.x = dest_rect.x + w;
        }
        dest_rect.y = dest_rect.y + h;
        dest_rect.x = START_X_OFFSET_PX;
    }
}

void ley::GameController::fadeMusic() {
    gm->audio()->fadeOutMusic();
}

void ley::GameController::playMainMenu() {
    gm->audio()->playMainMenu();
}

void ley::GameController::startPlayList() {
    gm->audio()->playPlaylist();
}

void ley::GameController::playNext() {
    gm->audio()->playNext();
}
