/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/15/2020
*/
#include "Block.h"

/* RAII */
ley::Block::Block(unsigned int x, unsigned int y, BlockType ty, bool c)
: type(ty), cf(c) {
    rect.x = x, rect.y = y;
    
    switch (type) {
        case BlockType::cube :
            rect.h = 2;
            rect.w = 2;
        break;
        case BlockType::tee :
            rect.h = 3;
            rect.w = 2;
        break;
        case BlockType::rLee :
            rect.h = 3;
            rect.w = 2;
        break;
        case BlockType::zee :
            rect.h = 2;
            rect.w = 3;
        break;
        case BlockType::mzee :
            rect.h = 2;
            rect.w = 3;
        break;
        case BlockType::lLee :
            rect.h = 3;
            rect.w = 2;
        break;
        case BlockType::line :
            rect.h = 4;
            rect.w = 1;
        break;
      
        default : ;
    }
    
    orientation = 0;
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

ley::BlockTexCode ley::Block::renderPart(unsigned int x /* width */, unsigned int y /* height */) {
    return block[y][x];
}

void ley::Block::rotate(bool direction) { //false for counterclockwise, true for clockwise  
    
    if(direction) {
        if(orientation == 3) {
            orientation = 0;
        }
        else {
            ++orientation;
        }
    } 
    else {
        if(orientation == 0) {
            orientation = 3;
        }
        else {
            --orientation;
        }
    }

    setBlock(type,this->orientation);
}

//return width for this height value.
int ley::Block::widthAtHeight(int height) {
    auto widthAtY = 0;
    for(auto k=0; k<rect.w; ++k) {
        ley::BlockTexCode partCheck = renderPart(k,height);
        if(partCheck != ley::BlockTexCode::O) {
            widthAtY++;
        }
    }
    return widthAtY;
}

//return height for this width value.
int ley::Block::heightAtWidth(int width) {
    auto heightAtX = 0;
    for(auto k=0; k<rect.h; ++k) {
        ley::BlockTexCode partCheck = renderPart(width,k);
        if(partCheck != ley::BlockTexCode::O) {
            heightAtX = k;
        }
    }
    return heightAtX;
}


//cube,tee,rLee,zee,mzee,lLee,line,empty
void ley::Block::setBlock(BlockType t, int o) {
    orientation = o;
    switch (t) {
        case BlockType::cube :
            block[0] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
            block[1] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
            block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
            block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
        break;
        case BlockType::tee :
            switch(o) {
                case 0 :
                    block[0] = { BlockTexCode::d, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::d, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h=3;
                    rect.w=2;
                break;
                case 1 :
                    block[0] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::d, BlockTexCode::O };
                    block[1] = { BlockTexCode::O, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h = 2;
                    rect.w = 3;
                break;
                case 2 :
                    block[0] = { BlockTexCode::O, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h=3;
                    rect.w=2;
                break;
                case 3 :
                    block[0] = { BlockTexCode::O, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::d, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h=2;
                    rect.w=3;
                break;
            }
        break;
        case BlockType::rLee :
            block[0] = { BlockTexCode::d, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
            block[1] = { BlockTexCode::d, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
            block[2] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
            block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
        break;
        case BlockType::zee :
            block[0] = { BlockTexCode::O, BlockTexCode::d, BlockTexCode::d, BlockTexCode::O };
            block[1] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
            block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
            block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
        break;
        case BlockType::mzee :
            block[0] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
            block[1] = { BlockTexCode::O, BlockTexCode::d, BlockTexCode::d, BlockTexCode::O };
            block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
            block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
        break;
        case BlockType::lLee :
            block[0] = { BlockTexCode::O, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
            block[1] = { BlockTexCode::O, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
            block[2] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
            block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
        break;
        case BlockType::line :
            block[0] = { BlockTexCode::d, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
            block[1] = { BlockTexCode::d, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
            block[2] = { BlockTexCode::d, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
            block[3] = { BlockTexCode::d, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
        break;
        default : ;
    }

    if(cf == 1) {
        block[0].fill(BlockTexCode::O);
        block[1].fill(BlockTexCode::O);
        block[2].fill(BlockTexCode::O);
        block[3].fill(BlockTexCode::O);
    }
}