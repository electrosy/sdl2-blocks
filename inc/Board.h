/* 
sdl2-blocks
Copyright (C) 2024 Steven Philley

Purpose: The Board object contains the blocks in play.
Date: Mar/30/2024
*/
#ifndef BOARD_H
#define BOARD_H

#include <array>
#include "../Block.h"
#include "../Renderable.h"
#include "../Textures.h"
#include "../inc/Command.h"

namespace ley {

class Board : public Renderable {

private:
    typedef std::vector<std::pair<ley::BlockTexCode,bool>> BoardType;
    int mWidth = 0;
    int mHeight = 0;
    int mBlockWidthPX = 0;
    int mBlockHeightPX = 0;
    int mBlockStartX = 0;
    int mBlockStartY = 0;
    BoardType mBoard;
    std::vector<int> test;
public:
    void debugOutput(bool inLayer);
    void assign(int inX, int inY);
    std::pair<ley::BlockTexCode, bool>* at(int inX, int inY);
    void setBlockSizeAndPos(int inWidth, int inHeight, int inX, int inY);
    void render(SDL_Renderer * r, bool d);
    void clear();
    bool canPut(Block& b, Command d);
    void setBlock(Block& b);
    void putBlock(Block& b);
    void fillLine(int l, std::pair<ley::BlockTexCode, bool> p);
 //   void shiftBoard(char start, char num);
};
}

#endif