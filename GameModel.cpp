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
: activeBlock(BLOCK_PUT_X,BLOCK_PUT_Y,BlockType::cube), oldBlock(BLOCK_PUT_X,BLOCK_PUT_Y,BlockType::empty) {
    clearBoard();
    oldBlock.setH(activeBlock.getRect().h);
    oldBlock.setW(activeBlock.getRect().w);
    putBlock(activeBlock);
}

ley::GameModel::~GameModel() {

}

/* Functions */
bool ley::GameModel::canMoveDown() {

}

bool ley::GameModel::canMoveLeft() {

}

bool ley::GameModel::canMoveRight() {

}

void ley::GameModel::clearBoard() {
    for(auto i = 0; i < BOARDSIZE_HEIGHT; ++i) {
        board[i].fill(BlockTexCode::O);
    }
}

void ley::GameModel::putBlock(Block& b) { //put the active block onto the board.
    SDL_Rect rect = b.getRect();
    for(int i = 0; i < rect.w; ++i) {
        for(int j = 0; j < rect.h; ++j) {
            // TODO check for collision with the other blocks that have been put in place. 

            //
            // NOTE board works backwards, this is where we reverse x and y. 
            //      this is common, see:
            // https://stackoverflow.com/questions/2203525/are-the-x-y-and-row-col-attributes-of-a-two-dimensional-array-backwards  
            board[rect.y+j][rect.x+i] = b.renderPart(i,j);
        }
    }
}

bool ley::GameModel::canPut(Block& b, Direction d) {
    SDL_Rect rect = b.getRect();

    unsigned int offset_x; unsigned int offset_y;
    switch (d) {
        case Direction::down :
            offset_x = 0;
            offset_y = rect.h;
        break;
        case Direction::right :
            offset_x = 1 + rect.w;
            offset_y = 0;
        break;
        case Direction::left :
            offset_x = -1;
            offset_y = 0;
        break; 
    }

    //if asked to go outside the bounds of the game board.
    if(rect.x+offset_x>BOARDSIZE_WIDTH || rect.y+offset_y>=BOARDSIZE_HEIGHT) {
        return 0; /*!*! EARLY EXIT !*!*/
    }

    for(int i = 0; i < rect.w; ++i) {
        for(int j = 0; j < rect.h; ++j) {
            //
            // NOTE board works backwards, this is where we reverse x and y. 
            //      this is common, see:
            // https://stackoverflow.com/questions/2203525/are-the-x-y-and-row-col-attributes-of-a-two-dimensional-array-backwards  

            //if the spot is not empty and we need to put a block piece there.
            if ( (board[rect.y+j+offset_y][rect.x+i+offset_x] != BlockTexCode::O)  && (b.renderPart(i,j) != BlockTexCode::O) ) {
               return 0; /*!*! EARLY EXIT !*!*/
            } else { 
                return 1; /*!*! EARLY EXIT !*!*/
            }
        }
    }
}

void ley::GameModel::downExpired() {

}
void ley::GameModel::clearOldBlock() {
    putBlock(oldBlock);
}
void ley::GameModel::moveBlock(Direction d) {
    switch (d) {
        case Direction::down :
            if (canPut(activeBlock, Direction::down)) {
                activeBlock.moveDown(); //move the active block down
                clearOldBlock(); //Clear out the space where the active block was
                oldBlock.moveDown(); //Move the oldBlock down as well so it will clear correctly next time.
            }
        break;

        case Direction::left :
            if (canPut(activeBlock, Direction::left)) {
                activeBlock.moveLeft();
                clearOldBlock();
                oldBlock.moveLeft();
            }
        break;

        case Direction::right :
            if (canPut(activeBlock, Direction::right)) {
                activeBlock.moveRight();
                clearOldBlock();
                oldBlock.moveRight();
            }
        break;

        default : ;
    }

    putBlock(activeBlock);
}

void ley::GameModel::debugBoard() {

    std::string sRow = "";
    for(auto row : board) {
            sRow = ""; //clear
        for(auto column : row) {
            sRow += TEXCODE_CHAR.find(column)->second + ".";
        }

        SDL_Log(sRow.c_str());
    }
}