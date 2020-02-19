/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/15/2020
*/
#include <array>

#include <SDL2/SDL.h>

#include "GameModel.h"

/* RAII */
ley::GameModel::GameModel()
: activeBlock(0,4,0) {
    clearBoard();

   putActive();
}

ley::GameModel::~GameModel() {

}

/* Functions */
void ley::GameModel::clearBoard() {
    for(auto i = 0; i < BOARDSIZE_HEIGHT; ++i) {
        for(auto j = 0; j < BOARDSIZE_WIDTH; ++j) {
            board[i][j] = "0";
        }
    }
}
void ley::GameModel::putActive() { //put the active block onto the board.
    
    SDL_Rect rect = activeBlock.getRect();

    for(int i = 0; i < rect.h; ++i) {
        for(int j = 0; j < rect.w; ++j) {
          board[rect.x+i][rect.y+j] = activeBlock.renderPart(i,j);
        }
    }
}

void ley::GameModel::downExpired() {

}
void ley::GameModel::moveBlock() {
    
}

void ley::GameModel::isBottom() {

}
void ley::GameModel::debugBoard() {
    

    std::string sRow = "";
    for(auto row : board) {
            sRow = ""; //clear
        for(auto column : row) {
            sRow += column + ".";
        }

        SDL_Log(sRow.c_str());
    }
}
