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
ren(v->getRenderer()), 
gm(g),
firstTimer(ren,3000,{10,300,100,50}),
secondTimer(ren,2500,{10,400,100,25}),
thirdTimer(ren,1000,{10,425,100,30}),
fourthTimer(ren,333,{10,455,100,5}),
fallTimer(ren,1000,{ley::START_X_OFFSET_PX-1,641,302,2}) {

    v->addRenderable(true, &firstTimer);
    v->addRenderable(true, &secondTimer);
    v->addRenderable(true, &thirdTimer);
    v->addRenderable(true, &fourthTimer);
    v->addRenderable(false, &fallTimer);

    gameStateMachine.changeState(new ley::IntroState());
}

ley::GameController::~GameController() {
}
/* Accessors */
ley::Timer* ley::GameController::getFallTimer() {
    return &fallTimer;
}
/* Functions */
void ley::GameController::runFrame(bool autoRestart, double newTime) {
    firstTimer.runFrame();
    secondTimer.runFrame();
    thirdTimer.runFrame();
    fourthTimer.runFrame();

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

void ley::GameController::setState(int statenum) {
    switch(statenum) {
      //  case 0 : 
    }
}
