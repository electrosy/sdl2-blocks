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

    audSystem.playIntro();
}

ley::GameController::~GameController() {
}
/* Accessors */

/* Functions */
void ley::GameController::runGameLoop(ley::HighScores &hs) {

    fadeMusic(); // finish up the intro music

    gameStateMachine.pushState(new ley::PlayState(mVideoSystem, gm));

    bool fs = mVideoSystem->fullScreen();
    SDL_Log("Starting Game loop!");
    
    bool gameOverState = false; // TODO we need to make this work without this extra 
                                    // gameOverState flag/token which indicates if the
                                    // state has been updated once the game resumes.
                                    // It would be better to just push/pop the states directly.
    while(gm->isGameRunning() || gm->programRunning()) {
        /**** MUSIC ****/
        //TODO startPlayList should prbably not be called every frame.
        startPlayList(); //start the main playlist for game play

        /**** RENDER ****/
        mVideoSystem->render();
        renderBoard();
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

        if(fs != mVideoSystem->fullScreen()) {
            mVideoSystem->setFullScreen(fs);
        }

        if(command == ley::Command::decreaseVolume) {
            audSystem.decreaseVolume();
        }

        if(command == ley::Command::increaseVolume) {
            audSystem.increaseVolume();
        }

        gameStateMachine.update(command);

        //Only allow paused if the game is not over.
        if(!gameOverState) {
            if(command == ley::Command::pause && gm->isPaused()) {
                audSystem.playSfx();
                SDL_Log("Game Paused!");
                gameStateMachine.pushState(new ley::PauseState(mVideoSystem, gm));
            }

            if(command == ley::Command::pause && !gm->isPaused()) {
                audSystem.playSfx();
                SDL_Log("Game Resumed");
                gameStateMachine.popState();
            }
        }

        //TODO should not use an extra flag for game over, should use the state machine directly.    
        if(!gm->isGameRunning() && gameOverState == false) {
            gameOverState = true;
            setHighScores(hs);
            gameStateMachine.pushState(new ley::GameOverState(mVideoSystem, gm));
        }

        if(gm->isGameRunning() && gameOverState == true) {
            gameOverState = false;
            gameStateMachine.popState(); // remove the game over state.
        }
                
        /**** CLEAR ****/
        mVideoSystem->clear(); //clear the backbuffer
        
        /**** LOCK FRAME RATE ****/
        mVideoSystem->frameDelay();
    }

    gameStateMachine.popState();

    /**** CLEAN UP ****/
    runCleanUp();
}
void ley::GameController::setHighScores(ley::HighScores &hs) {
    //push on the new high score
    hs.push(gm->getScore(), "Steve", gm->getLevel(), gm->getLines());
    hs.write();
    hs.setClean(false);
}

void ley::GameController::runCleanUp() {
    gm->resetGame();
    fadeMusic();
}

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
void ley::GameController::runIntros() {
    runIntro("sdl", {400,170,414,240}, 1);
    runIntro("itlogo", {400,155,400,400}, 1);
    fadeMusic();
}

//TODO runIntroScreens need better FPS throttling.
void ley::GameController::runIntro(std::string t, SDL_Rect r, double fpsDelay) {
    /**** Intro Screen Loop ****/
    bool intro = true;
    SDL_Texture* test = nullptr;
    test = TextureManager::Instance()->getTexture(t);
    SDL_SetTextureBlendMode(test,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(test, 255);

    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    src_rect.x = 0; src_rect.y = 0; src_rect.h = r.h ; src_rect.w = r.w;
    dest_rect.x = r.x; dest_rect.y = r.y; dest_rect.h = r.h; dest_rect.w = r.w;

    unsigned int alphaFrameIndex = 0;
    bool faddedin = false;
    char fadespeed = 10;
    while(intro == true) {
        if(!faddedin) {
            if(( SDL_GetTicks()  % fadespeed  ) == 0 ) {
                if(alphaFrameIndex < 255) {
                    alphaFrameIndex++;
                }
                else {
                    faddedin = true;
                }
            }
        } else {
             if(( SDL_GetTicks()  % fadespeed  ) == 0 ) {
                 if(alphaFrameIndex != 0) {  
                    alphaFrameIndex--;
                 }
            }
        }
        SDL_SetTextureAlphaMod(test, alphaFrameIndex);
        SDL_RenderCopy(mVideoSystem->getRenderer(), test, &src_rect, &dest_rect);
        mVideoSystem->present();
        if(mInputSystem.pollTitleEvents(intro) == ley::Command::down 
            || (alphaFrameIndex < 10 && faddedin)) {
            intro = false;
        }
        mVideoSystem->clear();

        SDL_Delay(fpsDelay);
    }

}

void ley::GameController::fadeMusic() {
    audSystem.fadeOutMusic();
}

void ley::GameController::playMainMenu() {
    audSystem.playMainMenu();
}

void ley::GameController::startPlayList() {
    audSystem.playPlaylist();
}

void ley::GameController::playNext() {
    audSystem.playNext();
}
