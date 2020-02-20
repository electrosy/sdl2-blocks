/* 
Author: Steven Philley
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

namespace ley {

enum class BlockType {cube,tee,rLee,zee,mzee,lLee,line,empty};
enum class BlockTexCode {a,b,c,d,e,f,g,h,i,j,k,l,O};

const std::map<BlockTexCode, std::string> TEXCODE_CHAR {
    std::make_pair (BlockTexCode::a, "a"),
    std::make_pair (BlockTexCode::b, "b"),
    std::make_pair (BlockTexCode::c, "c"),
    std::make_pair (BlockTexCode::d, "d"),
    std::make_pair (BlockTexCode::e, "e"),
    std::make_pair (BlockTexCode::f, "f"),
    std::make_pair (BlockTexCode::g, "g"),
    std::make_pair (BlockTexCode::h, "h"),
    std::make_pair (BlockTexCode::i, "i"),
    std::make_pair (BlockTexCode::j, "j"),
    std::make_pair (BlockTexCode::k, "k"),
    std::make_pair (BlockTexCode::l, "l"),
    std::make_pair (BlockTexCode::O, "O")
  };

class Block {

private:
    BlockType type;
    unsigned int orientation; // 0-3 - rotating to the left piece points down,right,up,left
    SDL_Rect rect; //Position and dimension    
    std::array<std::array<BlockTexCode, 4>,4> block;
    void setBlock(BlockType);
public:

    /* RAII */
    Block(unsigned int, unsigned int, BlockType);
    ~Block();

    /* Accessors */
    SDL_Rect getRect() {return rect;}; // TODO this should probably return a const
    const BlockType getType() {return type;};
    void setH(unsigned int h) {rect.h = h;};
    void setW(unsigned int w) {rect.w = w;};
    
    /* Functions */
    BlockTexCode renderPart(unsigned int, unsigned int); /* Return true or false depending on if the block would take up the x,y space */
    void moveDown();
    void moveLeft();
    void moveRight();
};

}
#endif
