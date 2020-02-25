/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/15/2020
*/
#include "Block.h"

/* RAII */
ley::Block::Block(unsigned int x, unsigned int y, BlockType ty)
: type(ty) {
    rect.x = x, rect.y = y;

    switch (type) {
        case BlockType::cube :
            rect.h = 2;
            rect.w = 2;
        break;
        case BlockType::empty :
            rect.h = 2;
            rect.w = 2;
        break;
        default : ;
    }

    setBlock(type);
}

ley::Block::~Block() {
    
}

/* Functions */
void ley::Block::moveDown() {
    rect.y = rect.y + 1;
}

void ley::Block::moveLeft() {
    rect.x = rect.x - 1;
}

void ley::Block::moveRight() {
    rect.x = rect.x + 1;
}

ley::BlockTexCode ley::Block::renderPart(unsigned int x, unsigned int y) {
    return block[x][y];
}

void ley::Block::setBlock(BlockType t) {

    switch (t) {
        case BlockType::cube :
            block[0][0] = BlockTexCode::d; block[0][1] = BlockTexCode::d; block[0][2] = BlockTexCode::O; block[0][3] = BlockTexCode::O;
            block[1][0] = BlockTexCode::d; block[1][1] = BlockTexCode::d; block[1][2] = BlockTexCode::O; block[1][3] = BlockTexCode::O;
            block[2][0] = BlockTexCode::O; block[2][1] = BlockTexCode::O; block[2][2] = BlockTexCode::O; block[2][3] = BlockTexCode::O;
            block[3][0] = BlockTexCode::O; block[3][1] = BlockTexCode::O; block[3][2] = BlockTexCode::O; block[3][3] = BlockTexCode::O;
            break;
        case BlockType::empty :
            block[0][0] = BlockTexCode::O; block[0][1] = BlockTexCode::O; block[0][2] = BlockTexCode::O; block[0][3] = BlockTexCode::O;
            block[1][0] = BlockTexCode::O; block[1][1] = BlockTexCode::O; block[1][2] = BlockTexCode::O; block[1][3] = BlockTexCode::O;
            block[2][0] = BlockTexCode::O; block[2][1] = BlockTexCode::O; block[2][2] = BlockTexCode::O; block[2][3] = BlockTexCode::O;
            block[3][0] = BlockTexCode::O; block[3][1] = BlockTexCode::O; block[3][2] = BlockTexCode::O; block[3][3] = BlockTexCode::O;
            break;
        default : ;
    }
}