/* 
sdl2-blocks
Copyright (C) 2024 Steven Philley

Purpose: A GameModel stores a representation of the game state.
Date: Mar/30/2024
*/
#ifndef BOARD_H
#define BOARD_H

#include <array>
#include "../Block.h"



namespace ley {



class Board {

private:
    typedef std::vector<std::pair<ley::BlockTexCode,bool>> BoardType;
    int mWidth = 0;
    int mHeight = 0;
    BoardType mBoard;
    std::vector<int> test;
public:
    void debugOutput();
    void assign(int inX, int inY);
    std::pair<ley::BlockTexCode, bool>* at(int inX, int inY);
};
}

#endif