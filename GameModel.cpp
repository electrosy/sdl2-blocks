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
: activeBlock(BLOCK_PUT_X,BLOCK_PUT_Y,BlockType::cube), oldBlock(BLOCK_PUT_X,BLOCK_PUT_Y,BlockType::cube,1),
    debug_phase(0) {
    clearBoard();
    oldBlock.setH(activeBlock.getRect().h);
    oldBlock.setW(activeBlock.getRect().w);
    putBlock(activeBlock);
}

ley::GameModel::~GameModel() {

}

/* Functions */
std::array<std::array<std::pair<ley::BlockTexCode,bool>, ley::BOARDSIZE_WIDTH>, ley::BOARDSIZE_HEIGHT >*
ley::GameModel::getBoard() {

}

void ley::GameModel::clearBoard() {
    for(auto i = 0; i < BOARDSIZE_HEIGHT; ++i) {
        board[i].fill(std::make_pair(BlockTexCode::O,false));
    }
}

// TODO This function should probably go in the controller?
void ley::GameModel::putBlock(Block& b) { //put the active block onto the board.
    SDL_Rect rect = b.getRect();
    for(int i = 0; i < rect.w; ++i) {
        for(int j = 0; j < rect.h; ++j) {
            // NOTE board works backwards, this is where we reverse x and y. 
            //      this is common, see:
            // https://stackoverflow.com/questions/2203525/are-the-x-y-and-row-col-attributes-of-a-two-dimensional-array-backwards 
            //
            BlockTexCode renderPart = b.renderPart(i,j);

            if(board[rect.y+j][rect.x+i].second == false) {
                board[rect.y+j][rect.x+i].first = renderPart;
            }
        }
    }
}

// **check for collision with the other blocks that have been put in place, 
//   also checks for collision with game board boundry
bool ley::GameModel::canPut(Block& b, Direction d) { // TODO canput() should probably go into the game controller
/* Check borders */
    unsigned int offset_x = 0;
    unsigned int offset_y = 0;
    SDL_Rect rect = b.getRect();
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

    bool canPut = true;
    //Iterate through the block and check and see if the board is empty where we need to put a block part.
    //Check if there is already a block along the edge of the existing block.
    switch (d) {
        case Direction::down : // check across x (x+n)
           for(int i = 0; i < rect.w; ++i) {
               for(int j = 0; j < rect.h; ++j) {
                    //if there is a block piece to put,then check to see if it can be put.
                    BlockTexCode renderablePart = b.renderPart(i, j);
                    BlockTexCode boardPart = board[rect.y + b.heightAtWidth(i) + 1][rect.x + i].first;
                    if( (renderablePart != BlockTexCode::O)//we have a part to render.
                        && (boardPart != BlockTexCode::O)//there is an empty space on the board.
                    ) {
                            canPut = false;
                    }
               }
           }
        break;
        case Direction::right : // check across y (y+n)
            for(int i = 0; i < rect.w; ++i) {
               for(int j = 0; j < rect.h; ++j) {
                    //if there is a block piece to put,then check to see if it can be put.
                    BlockTexCode renderablePart = b.renderPart(i, j);
                    auto widthAtHeight = b.widthAtHeight(j);
                    bool boardPart = board[rect.y + j][rect.x + widthAtHeight].second;
                    if( (renderablePart != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                            canPut = false;
                    }
               }
           }
        break;
        case Direction::left : // check across y (y-n)
            for(int i = 0; i<rect.h; ++i) {
               if(board[rect.y+offset_y+i][rect.x+offset_x].first != BlockTexCode::O) {
                   canPut = false; 
               }
           }
        break;
        
        break; // TODO extra break??
        default : break;
    }

    return canPut;
}

void ley::GameModel::downExpired() {

}
void ley::GameModel::clearOldBlock() {
    putBlock(oldBlock);
}

void ley::GameModel::newBlock() {
    SDL_Log("New Block");
  //  activeBlock = Block(4,0,BlockType::tee,false);
  //  oldBlock = Block(4,0,BlockType::tee,true);


    switch(debug_phase) {
        case 0 : 
            activeBlock = Block(4,0,BlockType::tee,false);
            oldBlock = Block(4,0,BlockType::tee,true);
        break;
        case 1 : 
            activeBlock = Block(4,0,BlockType::rLee,false);
            oldBlock = Block(4,0,BlockType::rLee,true);
        break;
        case 2 : 
            activeBlock = Block(4,0,BlockType::zee,false);
            oldBlock = Block(4,0,BlockType::zee,true);
        break;
        case 3 : 
            activeBlock = Block(4,0,BlockType::mzee,false);
            oldBlock = Block(4,0,BlockType::mzee,true);
        break;
        case 4 : 
            activeBlock = Block(4,0,BlockType::lLee,false);
            oldBlock = Block(4,0,BlockType::lLee,true);
        break;
        case 5 : 
            activeBlock = Block(4,0,BlockType::line,false);
            oldBlock = Block(4,0,BlockType::line,true);
        break;
        default :
            activeBlock = Block(4,0,BlockType::tee,false);
            oldBlock = Block(4,0,BlockType::tee,true);
        break;
    }

    ++debug_phase;
}

//TODO this function should probably go in the controller
//Iterate through the height and width of the block and set
//the board action layer elements to isset.
void ley::GameModel::setBlock() {
    SDL_Rect blockRect = activeBlock.getRect();
    for(auto i=0; i<blockRect.w; ++i) {
        for(auto j=0; j<blockRect.h; ++j) {
            if(activeBlock.renderPart(i,j) != BlockTexCode::O) {
                board[blockRect.y + j][blockRect.x + i].second = true;
            }
        }
    }
}

//TODO this function should probably go in the controller
void ley::GameModel::moveBlock(Direction d) {
    switch (d) {
        case Direction::down :
            if (canPut(activeBlock, Direction::down)) {
                activeBlock.moveDown(); //move the active block down
                clearOldBlock(); //Clear out the space where the active block was
                oldBlock.moveDown(); //Move the oldBlock(clearer) down as well so it will clear correctly next time.
            } else { setBlock(); newBlock(); return; /*!* EARLY EXIT *!*/ }
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

void ley::GameModel::debugBoard(bool setLayer) {
    std::string sRow = "";
    for(auto row : board) {
            sRow = ""; //clear
        for(auto column : row) {
            if(setLayer) {
                sRow += std::to_string(column.second) + ".";
            } else {
                sRow += TEXCODE_CHAR.find(column.first)->second + ".";
            }
        }
        SDL_Log(sRow.c_str());
    }
}