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
ley::GameController::GameController(SDL_Renderer* r, ley::GameModel* g)
: ren(r), gm(g) {
    
}

ley::GameController::~GameController() {

}
/* Accessors */


/* Functions */
void ley::GameController::renderBoard(SDL_Texture* t) {
    
    int startX = 200;
    int startY = 40;

    //get width and height of the texture
    int w, h;SDL_QueryTexture(t, NULL, NULL, &w, &h);

    SDL_Rect start_rect;
    start_rect.x = 0; start_rect.y = 0;
    start_rect.h = 20; start_rect.w = 20;

    SDL_Rect dest_rect;
    dest_rect.x = startX; dest_rect.y = startY;
    dest_rect.h = h; dest_rect.w = w;

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
        dest_rect.x=startX;
    }
}


void ley::GameController::runFrame(ley::Font* f) {  
    f->updateMessage("Score " + std::to_string(int(gm->getScore())));
}

void ley::GameController::goNext() {

}
