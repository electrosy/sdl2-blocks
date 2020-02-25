/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

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
std::array<std::array<ley::BlockTexCode, ley::BOARDSIZE_WIDTH>, ley::BOARDSIZE_HEIGHT >*
ley::GameModel::getBoard() {

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

/* Check borders */
    unsigned int offset_x = 0; 
    unsigned int offset_y = 0;
    
    switch (d) {
        case Direction::down :
            offset_x = 0;
            offset_y = rect.h + 1;
        break;
        case Direction::right :
            offset_x = rect.w + 1;
            offset_y = 0;
        break;
        case Direction::left :
            offset_x = -1;
            offset_y = 0;
        break;
        default : break;
    }
    //if asked to go outside the bounds of the game board.
    if( (rect.x+offset_x>BOARDSIZE_WIDTH) || (rect.y+offset_y>BOARDSIZE_HEIGHT) ) {
        return false; /*!*! EARLY EXIT !*!*/  //hit the border.
    } /* End Check Borders */

    //Check if there is already a block along the edge of the existing block.
    switch (d) {
        case Direction::down : // check across x (x+n)
           for(int i = 0; i<rect.w; ++i) {
               if(board[rect.y+offset_y-1][rect.x+offset_x+i] != BlockTexCode::O) {
                   return false; /*!*! EARLY EXIT !*!*/
               }
           }
        break;
        case Direction::right : // check across y (y+n)
            for(int i = 0; i<rect.h; ++i) {
               if(board[rect.y+offset_y+i][rect.x+offset_x-1] != BlockTexCode::O) {
                   return false; /*!*! EARLY EXIT !*!*/
               }
           }
        break;
        case Direction::left : // check across y (y-n)
            for(int i = 0; i<rect.h; ++i) {
               if(board[rect.y+offset_y+i][rect.x+offset_x] != BlockTexCode::O) {
                   return false; /*!*! EARLY EXIT !*!*/
               }
           }
        break;
        
        break;
        default : break;
    }

    return true;
}

void ley::GameModel::downExpired() {

}
void ley::GameModel::clearOldBlock() {
    putBlock(oldBlock);
}

void ley::GameModel::newBlock() {
    SDL_Log("New Block");
    activeBlock = Block(4,0,BlockType::cube);
    oldBlock = Block(4,0,BlockType::empty);
   // oldBlock.getRect().x = 2; oldBlock.getRect().y = 2;
}

void ley::GameModel::moveBlock(Direction d) {
    switch (d) {
        case Direction::down :
            if (canPut(activeBlock, Direction::down)) {
                activeBlock.moveDown(); //move the active block down
                clearOldBlock(); //Clear out the space where the active block was
                oldBlock.moveDown(); //Move the oldBlock(clearer) down as well so it will clear correctly next time.
            } else { newBlock(); return; /*!* EARLY EXIT *!*/ }
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