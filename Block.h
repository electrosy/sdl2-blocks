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
#include <SDL2/SDL.h>

namespace ley {

class Block {

private:
    unsigned int type; // the block type, 0,square,1,ear,2,rL,3,lSnake,4,rSnake,5,lL,6,line
    unsigned int orientation; // 0-3 - rotating to the left piece points down,right,up,left
    SDL_Rect rect; //Position and dimension    
    std::array<std::array<std::string, 4>,4> block;
    void setBlock(unsigned int);
public:

    Block(unsigned int, unsigned int, unsigned int);
    ~Block();

    SDL_Rect getRect() {return rect;};
    void moveDown();
    /* Return true or false depending on if the block would take up the x,y space */
    std::string renderPart(unsigned int, unsigned int);
};

}
#endif
