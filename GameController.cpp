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

const auto NEXTBLOCK_OFFSET_Y_PX = 10;

/* RAII */
ley::GameController::GameController(SDL_Renderer* r, ley::GameModel* g)
: ren(r), gm(g) {
    
}

ley::GameController::~GameController() {

}
/* Accessors */


/* Functions */
void ley::GameController::renderBoard(/*SDL_Texture* t*/) {

    //get width and height of the texture
    int w = 30, h = 30; //SDL_QueryTexture(t, NULL, NULL, &w, &h);

    SDL_Rect start_rect;
    start_rect.x = 0; start_rect.y = 0;
    start_rect.h = h; start_rect.w = w;

    SDL_Rect dest_rect;
    dest_rect.x = START_X_OFFSET_PX; dest_rect.y = BOARD_OFFSET_PX;
    dest_rect.h = h; dest_rect.w = w;


//TODO - Loop 1 and Loop 2 can probably be rafactored together.
    // Output the nextBlock from the game model
    SDL_Texture* blockBits = nullptr;
    SDL_Rect next_dest_rect; 
             next_dest_rect.x = 20; next_dest_rect.y = 40 + NEXTBLOCK_OFFSET_Y_PX;
             next_dest_rect.h = h; next_dest_rect.w = w;
    ley::Block nextBlock = gm->getNextBlock();
    for(auto row : nextBlock.getBlockParts()) {
        for(auto column : row) {
            if(column == BlockTexCode::d) {
                blockBits = TextureManager::Instance()->getTexture("d");
                SDL_RenderCopy(ren, blockBits, &start_rect, &next_dest_rect);
            }
            next_dest_rect.x = next_dest_rect.x + w;
        }
        next_dest_rect.y = next_dest_rect.y + h;
        next_dest_rect.x = 20;
    }

// TODO loop2 refactor with loop 1 above
    // Loop through the game model and output a representation to the video screen.
    SDL_Texture* test = nullptr;
    for(auto row : *gm->getBoard()) {
        for(auto column : row) {
            if(column.first == BlockTexCode::d) {
                test = TextureManager::Instance()->getTexture("d");
                SDL_RenderCopy(ren, test, &start_rect, &dest_rect);
            }
            dest_rect.x = dest_rect.x + w;
        }
        dest_rect.y = dest_rect.y + h;
        dest_rect.x=START_X_OFFSET_PX;
    }
}


void ley::GameController::runFrame(ley::Font* f) {  
    f->updateMessage("Score " + std::to_string(int(gm->getScore())));
}

void ley::GameController::goNext() {

}
