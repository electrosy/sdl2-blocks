/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/18/2020
*/
#include <SDL2/SDL.h>
#include "GameController.h"
#include "Textures.h"

typedef ley::Textures TextureManager;

/* RAII */
ley::GameController::GameController(SDL_Renderer* r, ley::GameModel* g)
: ren(r), gm(g) {
    
}

ley::GameController::~GameController() {

}
/* Functions */
void ley::GameController::renderBoard(SDL_Texture* t) {
    SDL_Rect start_rect;
    start_rect.x = 0; start_rect.y = 0;
    start_rect.h = 20; start_rect.w = 20;

    SDL_Rect dest_rect;
    dest_rect.x = 400; dest_rect.y = 400;
    dest_rect.h = 20; dest_rect.w = 20;

    SDL_Rect dest_rect2;
    dest_rect2.x = 400; dest_rect2.y = 420;
    dest_rect2.h = 20; dest_rect2.w = 20;

    SDL_Texture* test = TextureManager::Instance()->getTexture("d");

    // TODO Loop through the game model and output a representation to 
    //      the video screen.

    SDL_RenderCopy(ren, test, &start_rect, &dest_rect);
    SDL_RenderCopy(ren, test, &start_rect, &dest_rect2);
}


void ley::GameController::runFrame() {

}

void ley::GameController::goNext() {

}
