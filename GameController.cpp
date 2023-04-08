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
gm(g),
fallTimer(ren,1000,{ley::START_X_OFFSET_PX-1,641,302,2}) {

    mVideoSystem->addRenderable(false, &fallTimer);

    audSystem.playIntro();

    gameStateMachine.changeState(new ley::PlayState(v));
    gameStateMachine.loadRenderables();
}

ley::GameController::~GameController() {
}
/* Accessors */
ley::Timer* ley::GameController::getFallTimer() {
    return &fallTimer;
}
/* Functions */
void ley::GameController::runFrame(bool autoRestart, double newTime) {
    
    fallTimer.runFrame(autoRestart, newTime);
    gameStateMachine.update();
}
void ley::GameController::renderBoard(/*SDL_Texture* t*/) {
    //get width and height of the texture
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
        if(mInputSystem.pollTitleEvents(intro,(*gm)) == ley::Direction::down 
            || (alphaFrameIndex < 10 && faddedin)) {
            intro = false;
        }
        mVideoSystem->clear();

        SDL_Delay(fpsDelay);
    }

}

void ley::GameController::setState(int statenum) {
    switch(statenum) {
        case 0 : gameStateMachine.changeState(new ley::IntroState(mVideoSystem));
        break;

        case 1 : gameStateMachine.changeState(new ley::MenuState(mVideoSystem));
        break;

        case 2 : gameStateMachine.changeState(new ley::PlayState(mVideoSystem));
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
