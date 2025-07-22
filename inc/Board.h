/* 
sdl2-blocks
Copyright (C) 2024 Steven Philley

Purpose: The Board object contains the blocks in play.
Date: Mar/30/2024
*/
#ifndef BOARD_H
#define BOARD_H

#include <array>
#include "Block.h"
#include "../Renderable.h"
#include "../Textures.h"
#include "../inc/Command.h"
#include "../inc/Screen.h"

namespace ley {

const int BOARDSIZE_BUFFER = 3; // buffer above board for piece to come into play

const auto BOARD_POS_Y_PX = 40; //Board starting position y for block, when drawing border subtract 1px.
const auto BLOCK_START_POS_Y_PX = BOARD_POS_Y_PX - (BLOCKSIZE_PX*BOARDSIZE_BUFFER); //Subtract three rows from board start_y //Number of pixels to offset the board.

const auto NEXTBOX_PADDING_PX = 25; // padding from the game board
const auto NEXTBOX_SIZE_PX = (BLOCKSIZE_PX * 4) + 6; // four blocks can fit in the next border plus 5px padding.
const auto NEXTBOX_POS_Y_PX = BOARD_POS_Y_PX;

const auto NEXTBLOCK_OFFSET_Y_PX = 10;
const auto NEW_LVL_AT_LINES = 10;
auto const COMBO_POS_Y_PX = NEXTBOX_POS_Y_PX + NEXTBOX_SIZE_PX + 40;

const auto PTS_DROP = 5;
const auto PTS_2LINE_MULT = 5;
const auto PTS_3LINE_MULT = 15;
const auto PTS_4LINE_MULT = 45;

//changes based on board width and height
const auto BOARDSIZE_WIDTH = 10;
const auto BOARDSIZE_WIDTH_PX = BOARDSIZE_WIDTH * BLOCKSIZE_PX;
const auto BOARD_POS_X_PX =  (SCREEN_WIDTH / 2) - (BOARDSIZE_WIDTH_PX / 2);//490; //Board starting position x for block, when drawing border subtract 1px.
const auto NEXTBOX_POS_X_PX = BOARD_POS_X_PX - NEXTBOX_PADDING_PX - NEXTBOX_SIZE_PX;
auto const COMBO_POS_X_PX = NEXTBOX_POS_X_PX;

//changes based on board height
const auto BOARDSIZE_HEIGHT = 20 + BOARDSIZE_BUFFER; //add 3 additional blocks so tetrominos can enter play in the right place.
const auto BOARDSIZE_HEIGHT_PX = (BOARDSIZE_HEIGHT - BOARDSIZE_BUFFER) * BLOCKSIZE_PX;

class Board : public Renderable {

private:
    typedef std::vector<BoardCellType> BoardType;
    int mWidth = 0;
    int mHeight = 0;
    int mWidthPx = 0;
    int mHeightPx = 0;
    int mBlockWidthPx = 0;
    int mBlockHeightPx = 0;
    int mBoardPosXPx = 0;
    int mBlockStartY = 0;
    int mNextBoxPosXPx = 0;
    int mScorePosXPx = 0;
    BoardType mBoard; // 0 based
public:
    Board();
    void debugOutput(bool inLayer);
    void debugFill();
    void setSize(int inX, int inY);
    std::pair<ley::BlockTexCode, bool>* at(int inX, int inY); //The X and Y are 0 based.
    void render(SDL_Renderer * r, bool d);
    void clear();
    std::pair<bool, std::string> canPut(Block& b, Command d);
    void setBlock(Block& b);
    void putBlock(Block& b);
    void fillLine(int l, std::pair<ley::BlockTexCode, bool> p);
    
    int width() { return mWidth; };
    int height() { return mHeight; };
    int widthpx() { return mWidthPx; };
    int heightpx() { return mHeightPx; };
    int boardPosXPx() { return mBoardPosXPx; };
    int nextBoxPosXPx() { return mNextBoxPosXPx; };
    int scorePosXPx() { return mScorePosXPx; };

};
}

#endif