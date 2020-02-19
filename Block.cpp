/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/15/2020
*/
#include "Block.h"

/* RAII */
ley::Block::Block(unsigned int x, unsigned int y, unsigned int t)
: type(t) {
    rect.x = x, rect.y = y;

    if(type == 0) {
        rect.h = 2;
        rect.w = 2;
    }

    setBlock(t);
}

ley::Block::~Block() {
    
}

/* Functions */
std::string ley::Block::renderPart(unsigned int x, unsigned int y) {
    return block[x][y];
}

void ley::Block::setBlock(unsigned int type) {

    switch (type) {
        case 0:   
            block[0][0] = "*"; block[0][1] = "*"; block[0][2] = ""; block[0][3] = "";
            block[1][0] = "*"; block[1][1] = "*"; block[1][2] = ""; block[1][3] = "";
            block[2][0] = ""; block[2][1] = ""; block[2][2] = ""; block[2][3] = "";
            block[3][0] = ""; block[3][1] = ""; block[3][2] = ""; block[3][3] = "";
    }
}

void ley::Block::moveDown() {

    //TODO if not running into another block.
    ++rect.y;
}