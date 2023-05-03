/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: A Block is a representation of a blocktype
Date: Feb/15/2020
*/
#ifndef BLOCK_H
#define BLOCK_H

#include <utility>
#include <string>
#include <array>
#include <map>
#include <SDL2/SDL.h>

#include "Rand_int.h"

const auto BLOCK_START_X = 4;
const auto BLOCK_START_Y = 0;

namespace ley {

enum class BlockType {cube,tee,rLee,zee,mzee,lLee,line,empty};
// the letter 'O' means empty and is the default. Capital 'Z' means don't clear part if clear block.
enum class BlockTexCode {a,b,c,d,e,f,g,h,i,j,k,l,O,Z};

const std::map<BlockTexCode, std::string> TEXCODE_CHAR {
    std::make_pair (BlockTexCode::a, "a"), // 12 texture possibilities
    std::make_pair (BlockTexCode::b, "b"), //
    std::make_pair (BlockTexCode::c, "c"), //
    std::make_pair (BlockTexCode::d, "d"), //
    std::make_pair (BlockTexCode::e, "e"), //
    std::make_pair (BlockTexCode::f, "f"), //
    std::make_pair (BlockTexCode::g, "g"), //
    std::make_pair (BlockTexCode::h, "h"), //
    std::make_pair (BlockTexCode::i, "i"), //
    std::make_pair (BlockTexCode::j, "j"), //
    std::make_pair (BlockTexCode::k, "k"), //
    std::make_pair (BlockTexCode::l, "l"), //
    std::make_pair (BlockTexCode::O, "O"), //This is the empty texture blockpart.
  };

class Block {

private:
    BlockType type; //type empty == null block
    unsigned int orientation; // 0-3 - rotating to the left piece points right,down,left,up
    SDL_Rect rect; //Position and dimension
    std::array<std::array<BlockTexCode, 4>,4> block;
    bool cf; //clear flag, used for a clear block, to clean up the oldposition.
    ley::BlockTexCode getRandomTexture();
    void setBlock(BlockType,int = 0);

public:

    /* RAII */
    Block();
    Block(unsigned int, unsigned int, BlockType, bool = 0); //x,y,type,clear
    Block(const Block& b); //copy constructor
    ~Block();

    /* Accessors */
    SDL_Rect getRect() {return rect;}; // TODO this should probably return a const
    const BlockType getType() {return type;};
    const bool getClear() {return cf;};
    void setClear(bool c);
    void setH(unsigned int h) {rect.h = h;};
    void setW(unsigned int w) {rect.w = w;};
    void rotate(bool); // false for counterclockwise and true for clockwise.
    std::array<std::array<ley::BlockTexCode, 4>,4> getBlockParts();

    void operator= (const Block &b);
    
    /* Functions */
    BlockTexCode renderPart(unsigned int, unsigned int); /* Return true or false depending on if the block would take up the x,y space */
    void moveDown();
    void moveLeft();
    void moveRight();
    int max_x(int);
    int heightAtWidth(int);
    int widthAtHeight(int); // TODO this method isn't used, clean it up.
    void reset(); //return block to original position for restarting the game.


};



}
#endif
