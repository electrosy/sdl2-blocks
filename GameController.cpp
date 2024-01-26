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

    gm->UIInputFocus(ley::UIFocusChange::game);

    bool fs = mVideoSystem->fullScreen();
    SDL_Log("Starting Game loop!");

    while(gm->programRunning()) {
        mVideoSystem->frameStart(); //Need to call this at the begenning of the frame, for the frame delay.

        /**** RENDER ****/
        //Only render the game board if we are in PLAY, PAUSE or GAMEOVER.
        if(gameStateMachine.getStateId() == "PLAY" 
            || gameStateMachine.getStateId() == "PAUSE"
            || gameStateMachine.getStateId() == "GAMEOVER") {
            
            mVideoSystem->render();
            renderBoard();
        }
        gameStateMachine.render();
        mVideoSystem->renderTopLayer();
        
        /**** PRESENT ****/
        mVideoSystem->present(); // output to the video system.
        
        /**** GET INPUT ****/
        //pollEvents updates running and full screen flags

        if(gm->UIInputFocus() == ley::UIFocusChange::goto_textBox) {
            SDL_StartTextInput();
            gameStateMachine.textEntry()->toggleFocus();
            gm->UIInputFocus(ley::UIFocusChange::textBox);
        }
        else if(gm->UIInputFocus() == ley::UIFocusChange::goto_game){
            SDL_StopTextInput();
            gameStateMachine.textEntry()->toggleFocus();
            gm->UIInputFocus(ley::UIFocusChange::game);
        }

        ley::Command command = mainInput.pollEvents(fs, gameStateMachine.textEntry()->getTextBoxField());

        /**** INPUT PROCESSING ****/
        if(fs != mVideoSystem->fullScreen()) {
            mVideoSystem->setFullScreen(fs);
        }

        if(gm->UIInputFocus() == ley::UIFocusChange::game) {
            processCommands(command);
            gameStateMachine.update(command);
            processStates(command);
        }
        else if(gm->UIInputFocus() == ley::UIFocusChange::textBox) {
            //throw away the command but continue to run the game state machine
            //mVideoSystem->processTextBox(command == ley::Command::backspace ? ley::Character::backspace : ley::Character::none);
            gameStateMachine.textEntry()->processTextBox(command == ley::Command::backspace ? ley::Character::backspace : ley::Character::none);
            
            if(command == ley::Command::enter) {
                gm->UIInputFocus(ley::UIFocusChange::goto_game);
                gameStateMachine.commitUI();
            }
            
            gameStateMachine.update(ley::Command::none);
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
void ley::GameController::processCommands(ley::Command command) {
    if(command == ley::Command::nextSong) {
        playNext();
    }

    if(command == ley::Command::decreaseVolume) {
        gm->audio()->decreaseVolume();
    }

    if(command == ley::Command::increaseVolume) {
        gm->audio()->increaseVolume();
    }

    /**** DEBUG INPUT PROCESSING ****/
    if(command == ley::Command::debugkeystoggle) {
        gm->debugCommandsToggle();
    }

    //allow debug commands to be used
    if(gm->debugCommands()) {
        if(command == ley::Command::console) {
            gm->overlayToggle();
        }

        if(command == ley::Command::debugclear) {
            gm->clearBoard();
            gm->debugResetActiveBlock();
        }

        if(command == ley::Command::debugfill) {
            gm->debugFill();
        }

        if(command == ley::Command::debugnextlevel) {
            gm->debugNextLevel();
        }

        if(command == ley::Command::debugprevlevel) {
            gm->debugPrevLevel();
        }

        if(command == ley::Command::debugonlyline) {
            gm->debugOnlyLineToggle();
        }
    }
}
void ley::GameController::processStates(ley::Command command) {
    //NOTE game state machine update was here.

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
      //  setHighScores(gm->highScores());

        // if new high score
        if(gm->highScores()->isNewHigh(gm->getScore()) > 0) {
            gm->newHighScore(true);
        }
        
        gameStateMachine.pushState(new ley::GameOverState(mVideoSystem, gm));

        if(gm->newHighScore()) {
            gameStateMachine.pushState(new ley::HighScoresMenuState(mVideoSystem, gm));
        }
        
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
}
void ley::GameController::setHighScores(ley::HighScores* hs) {
    //push on the new high score
    hs->setHighScore(gm->getScore(), "Steve", gm->getLevel(), gm->getLines());
}

void ley::GameController::runCleanUp() {
    gm->resetGame();
    fadeMusic();
}
std::pair<int, int> ley::GameController::centerRectInPx(SDL_Rect outer, SDL_Rect inner) {
    
    int x = 0;
    int y = 0;

    x = (outer.w/2) - (inner.w/2);
    y = (outer.h/2) - (inner.h/2);
    
    return std::make_pair(x,y);
}

//TODO the board should be rendered in the state machine like everything else.
void ley::GameController::renderBoard(/*SDL_Texture* t*/) {
    //get width and height of the texture // TODO this should be dynamic based on image passed in
    int w = BLOCKSIZE_PX, h = BLOCKSIZE_PX; //SDL_QueryTexture(t, NULL, NULL, &w, &h);

    SDL_Rect start_rect;
    start_rect.x = 0;
    start_rect.y = 0;
    start_rect.h = h;
    start_rect.w = w;

    SDL_Rect dest_rect;
    dest_rect.x = BLOCK_START_POS_X_PX;
    dest_rect.y = BLOCK_START_POS_Y_PX;
    dest_rect.h = h;
    dest_rect.w = w;

    //TODO - Loop 1 and Loop 2 can probably be rafactored together.
    // Output the nextBlock from the game model
    
    ley::Block nextBlock = gm->getNextBlock();

    std::pair<int, int> pos = centerRectInPx({NEXTBOX_POS_X_PX, NEXTBOX_POS_Y_PX, NEXTBOX_SIZE_PX, NEXTBOX_SIZE_PX},
                                            {nextBlock.getRect().x, nextBlock.getRect().y, nextBlock.width()*BLOCKSIZE_PX, nextBlock.height()*BLOCKSIZE_PX});

    SDL_Rect next_dest_rect;
    next_dest_rect.x = NEXTBOX_POS_X_PX + pos.first;
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
                SDL_RenderCopy(ren, TextureManager::Instance()->getTexture(TEXCODE_CHAR.at(column)), 
                    &start_rect, &next_dest_rect);
            }
            next_dest_rect.x = next_dest_rect.x + w;
        }
        next_dest_rect.y = next_dest_rect.y + h;
        next_dest_rect.x = NEXTBOX_POS_X_PX + pos.first;
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
        dest_rect.x = BLOCK_START_POS_X_PX;
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
