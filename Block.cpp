/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/15/2020
*/
#include "Block.h"

/* RAII */
ley::Block::Block() {

}
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

ley::Block::Block(const Block& b) {
    type = b.type;
    orientation = b.orientation; // 0-3 - rotating to the left piece points right,down,left,up
    rect = b.rect; //Position and dimension
    cf = b.cf;
    setBlock(b.type);
}

void ley::Block::operator=(const Block& b) {
    type = b.type;
    orientation = b.orientation; // 0-3 - rotating to the left piece points right,down,left,up
    rect = b.rect; //Position and dimension
    cf = b.cf;
    setBlock(b.type);
}

ley::Block::~Block() {
    
}

void ley::Block::setClear(bool c) {
    cf = c;
    setBlock(type);
}

void ley::Block::reset() {
    
    rect.x = BLOCK_START_X;
    rect.y = BLOCK_START_Y;
}

std::array<std::array<ley::BlockTexCode, 4>,4> ley::Block::getBlockParts() {
    return block;
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
//Generate a random texture code.
ley::BlockTexCode ley::Block::getRandomTexture() {

    ley::BlockTexCode tex_code;
    
    ley::Rand_int rand0to5(0,5);

    switch(rand0to5()) {
        case 0 : tex_code = BlockTexCode::d;
            break;
        case 1 : tex_code = BlockTexCode::e;
            break;
        case 2 : tex_code = BlockTexCode::f;
            break;
        case 3 : tex_code = BlockTexCode::g;
            break;
        case 4 : tex_code = BlockTexCode::h;
            break;
        case 5 : tex_code = BlockTexCode::i;
            break;
    }

    return tex_code;
}

//cube,tee,rLee,zee,mzee,lLee,line,empty
void ley::Block::setBlock(BlockType t, int o) {
    orientation = o;

    switch (t) {
        case BlockType::cube :
            switch(o)
            case 0 :
            case 1 :
            case 2 :
            case 3 :
                block[0] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
                block[1] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O };
                block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
            break;
        break;
        case BlockType::tee :
            switch(o) {
                case 0 :
                    block[0] = { BlockTexCode::e, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::e, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::e, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h=3;
                    rect.w=2;
                break;
                case 1 :
                    block[0] = { BlockTexCode::e, BlockTexCode::e, BlockTexCode::e, BlockTexCode::O };
                    block[1] = { BlockTexCode::O, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h = 2;
                    rect.w = 3;
                break;
                case 2 :
                    block[0] = { BlockTexCode::O, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::e, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h=3;
                    rect.w=2;
                break;
                case 3 :
                    block[0] = { BlockTexCode::O, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::e, BlockTexCode::e, BlockTexCode::e, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h=2;
                    rect.w=3;
                break;
            }
        break;
        case BlockType::rLee :
             switch(o) {
                case 0 :
                    block[0] = { BlockTexCode::f, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::f, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::f, BlockTexCode::f, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h = 3;
                    rect.w = 2;
                break;
                case 1 :
                    block[0] = { BlockTexCode::f, BlockTexCode::f, BlockTexCode::f, BlockTexCode::O };
                    block[1] = { BlockTexCode::f, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h = 2;
                    rect.w = 3;
                break;
                case 2 :
                    block[0] = { BlockTexCode::f, BlockTexCode::f, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::O, BlockTexCode::f, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::f, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h = 3;
                    rect.w = 2;
                break; 
                case 3 :
                    block[0] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::f, BlockTexCode::O };
                    block[1] = { BlockTexCode::f, BlockTexCode::f, BlockTexCode::f, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h = 2;
                    rect.w = 3;
                break;
             }
        break;
        case BlockType::zee :
            switch(o) {
                case 0 :
                case 2 :
                    block[0] = { BlockTexCode::O, BlockTexCode::g, BlockTexCode::g, BlockTexCode::O };
                    block[1] = { BlockTexCode::g, BlockTexCode::g, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h = 2;
                    rect.w = 3;
                break;
                case 1 :
                case 3 :
                    block[0] = { BlockTexCode::g, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::g, BlockTexCode::g, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::g, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h = 3;
                    rect.w = 2;
                break;
            }
        break;
        case BlockType::mzee :
             switch(o) {
                case 0 :
                case 2 :
                    block[0] = { BlockTexCode::h, BlockTexCode::h, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::O, BlockTexCode::h, BlockTexCode::h, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                rect.h = 2;
                rect.w = 3;
                break;
                case 1 :
                case 3 :
                    block[0] = { BlockTexCode::O, BlockTexCode::h, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::h, BlockTexCode::h, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::h, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                rect.h = 3;
                rect.w = 2;
                break;
             }
        break;
        case BlockType::lLee :
            switch (o) {
            case 0 :
                block[0] = { BlockTexCode::O, BlockTexCode::i, BlockTexCode::O, BlockTexCode::O };
                block[1] = { BlockTexCode::O, BlockTexCode::i, BlockTexCode::O, BlockTexCode::O };
                block[2] = { BlockTexCode::i, BlockTexCode::i, BlockTexCode::O, BlockTexCode::O };
                block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                rect.h = 3;
                rect.w = 2;
            break;
            case 1 :
                block[0] = { BlockTexCode::i, BlockTexCode::i, BlockTexCode::i, BlockTexCode::O };
                block[1] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::i, BlockTexCode::O };
                block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                rect.h = 2;
                rect.w = 3;
            break;
            case 2 :
                block[0] = { BlockTexCode::i, BlockTexCode::i, BlockTexCode::O, BlockTexCode::O };
                block[1] = { BlockTexCode::i, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                block[2] = { BlockTexCode::i, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                rect.h = 3;
                rect.w = 2;
            break;
            case 3 :
                block[0] = { BlockTexCode::i, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                block[1] = { BlockTexCode::i, BlockTexCode::i, BlockTexCode::i, BlockTexCode::O };
                block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                rect.h = 2;
                rect.w = 3;
            break;
            }
        break;
        case BlockType::line :
            switch (o) {
                case 0 :
                case 2 :
                    block[0] = { BlockTexCode::j, BlockTexCode::j, BlockTexCode::j, BlockTexCode::j };
                    block[1] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h = 1;
                    rect.w = 4;
                break;

                case 1 :
                case 3 :
                    block[0] = { BlockTexCode::j, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[1] = { BlockTexCode::j, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[2] = { BlockTexCode::j, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    block[3] = { BlockTexCode::j, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    rect.h = 4;
                    rect.w = 1;
                break;
            }
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