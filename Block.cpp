/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/15/2020
*/
#include "Block.h"

ley::BlockFileDataMapType* ley::Block::mBlockDataPtr = nullptr;
/* RAII */
ley::Block::Block() {

}
ley::Block::Block(unsigned int x, unsigned int y, BlockNameType ty, bool c)
: mType(ty), mClearFlag(c) {
    
    mRect.x = x, mRect.y = y;
    mOrientation = 0;
    if(mReadBlockFromFile) {
        setBlockFromFile(mType, mOrientation, &mBlockData, mClearFlag);
    }
    else {
        setBlock(mType);
    }
}

ley::Block::Block(const Block& b) {
    mType = b.mType;
    mOrientation = b.mOrientation; // 0-3 - rotating to the left piece points right,down,left,up
    mRect = b.mRect; //Position and dimension
    mClearFlag = b.mClearFlag;
    
    if(mReadBlockFromFile) {
        setBlockFromFile(b.mType, mOrientation, &mBlockData, mClearFlag);
    }
    else {
        setBlock(b.mType);
    }
}

void ley::Block::operator=(const Block& b) {
    mType = b.mType;
    mOrientation = b.mOrientation; // 0-3 - rotating to the left piece points right,down,left,up
    mRect = b.mRect; //Position and dimension
    mClearFlag = b.mClearFlag;
    if(mReadBlockFromFile) {
        setBlockFromFile(b.mType, mOrientation, &mBlockData, mClearFlag);
    }
    else {
        setBlock(b.mType);
    }

}

ley::Block::~Block() {
    
}
void ley::Block::debugResetPos() {
    mRect.y = 0;

}

void ley::Block::setClear(bool c) {
    mClearFlag = c;

    if(mReadBlockFromFile) {
        mOrientation = 0;
        setBlockFromFile(mType, 0, &mBlockData, mClearFlag);
    }
    else {
        setBlock(mType);
    }
}

void ley::Block::reset() {
    
    mRect.x = BLOCK_START_X;
    mRect.y = BLOCK_START_Y;
}

std::array<std::array<ley::BlockTexCode, 5>,5> ley::Block::getBlockParts() {
    return mBlockData;
}


/* Functions */
void ley::Block::moveDown() {
    mRect.y = mRect.y + 1;
}

void ley::Block::moveLeft() {
    mRect.x = mRect.x - 1;
}

void ley::Block::moveRight() {
    mRect.x = mRect.x + 1;
}

ley::BlockTexCode ley::Block::renderPart(unsigned int x /* width */, unsigned int y /* height */) {
    return mBlockData[y][x];
}

bool ley::Block::rotate(bool direction) { //false for counterclockwise, true for clockwise  
    
    bool rotated = false;

    //early return if there is no other orientations for block, e.g. cube.
    // TODO we need to remove this hard coding for cube here so that the it will work with the block editor.
    // we need to calculate if the block can be rotated.
    /*
    if(type == ley::BlockNameType::cube) { 
        return false;
    }
        */
    if(!getCanRotate()) {
        return false;
    }

    if(direction) {
        if(mOrientation == 3) {
            mOrientation = 0;
        }
        else {
            ++mOrientation;
        }
    } 
    else {
        if(mOrientation == 0) {
            mOrientation = 3;
        }
        else {
            --mOrientation;
        }
    }

    if(mReadBlockFromFile) {
        setBlockFromFile(mType,this->mOrientation, &mBlockData, mClearFlag);
    }
    else {
        setBlock(mType,this->mOrientation);
    }

    rotated = true;

    return rotated;
}

// TODO this should be called only once when the block is created.
int ley::Block::height() {
    
    int height = 0;
    int gap = 0; // count gaps and add them at the end if there is a block at the end.
    bool countgap = false;
    for(auto i = 0; i < mRect.h; ++i) {
        if(widthAtHeight(i) > 0) {
            height++;
            height += gap;
            gap = 0;
            countgap = true;
        }
        else if (countgap) {
            gap++;
        }
    }

    return height;
}

// TODO this should be called only once when the block is created.
int ley::Block::width() {

    int width = 0;
    int gap = 0; // count gaps and add them at the end if there is a block at the end.
    bool countgap = false;
    for(auto i = 0; i < mRect.w; ++i) {
        if(heightAtWidth(i) > 0) {
            width++;
            width += gap;
            gap = 0;
            countgap = true;
        }
        else if (countgap) {
            gap++;
        }
    }

    return width;
}

//return width for this height value.
int ley::Block::widthAtHeight(int height) {
    auto widthAtY = 0;
    for(auto k=0; k<mRect.w; ++k) {
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
    for(auto k=0; k<mRect.h; ++k) {
        ley::BlockTexCode partCheck = renderPart(width,k);
        if(partCheck != ley::BlockTexCode::O) {
            heightAtX++;
        }
    }
    return heightAtX;
}
void ley::Block::setBlockDataPtr(BlockFileDataMapType* blockDataPtr) {
    mBlockDataPtr = blockDataPtr;
}

void ley::Block::loadSingleOrientation(std::string orientation, BlockDataType* blockData) {

    auto blockTexCodeFromString = [](char str_code) -> ley::BlockTexCode  {

        std::map<char, ley::BlockTexCode> strTexCodeMap;

        strTexCodeMap.emplace('O', ley::BlockTexCode::O);
        strTexCodeMap.emplace('a', ley::BlockTexCode::a);
        strTexCodeMap.emplace('b', ley::BlockTexCode::b);
        strTexCodeMap.emplace('c', ley::BlockTexCode::c);
        strTexCodeMap.emplace('d', ley::BlockTexCode::d);
        strTexCodeMap.emplace('e', ley::BlockTexCode::e);
        strTexCodeMap.emplace('f', ley::BlockTexCode::f);
        strTexCodeMap.emplace('g', ley::BlockTexCode::g);
        strTexCodeMap.emplace('h', ley::BlockTexCode::h);
        strTexCodeMap.emplace('i', ley::BlockTexCode::i);
        strTexCodeMap.emplace('j', ley::BlockTexCode::j);
        strTexCodeMap.emplace('k', ley::BlockTexCode::k);
        strTexCodeMap.emplace('l', ley::BlockTexCode::l);
        strTexCodeMap.emplace('Z', ley::BlockTexCode::Z);

        return strTexCodeMap[str_code];
    };

    // TODO BLOCK_SIZE is assumed to be square, maybe we should define a height and width.
    for(int i = 0; i < BLOCK_SIZE; ++i) {

        std::string str_data = (*mBlockDataPtr)[orientation + std::to_string(i)];
        
        for(int j = 0; j < BLOCK_SIZE; ++j) {
            
                if(str_data.size() >= j + 1) {
                    (*blockData)[i][j] = blockTexCodeFromString(str_data[j]);
                }
            
        }
    }
}

int ley::Block::bottomEdgeOfOrientation(BlockDataType* blockData) {
    
    for(int i = BLOCK_SIZE - 1; i >= 0; --i) {
        for(int j = BLOCK_SIZE - 1; j >= 0; --j) {
            if((*blockData)[i][j] != ley::BlockTexCode::O) {
                return i + 1; //the row that has the first tex code + 1;
            }
        }
    }

    return -1;
}

int ley::Block::rightEdgeOfOrientation(BlockDataType* blockData) {

    // TODO BLOCK_SIZE_WIDTH
    for(int i = BLOCK_SIZE - 1; i >= 0; --i) {
        for(int j = BLOCK_SIZE - 1; j >= 0; --j) {
            if((*blockData)[j][i] != ley::BlockTexCode::O) {
                
                return i + 1; //the column that has the first tex code + 1;
            }
        }
    }

    return -1;
}


bool ley::Block::canRotate(std::string blockCharName, BlockFileDataMapType* inBlockDataFileMapPtr) {

    return (*inBlockDataFileMapPtr)[blockCharName + "*"] == "yes" ? true : false;
}

void ley::Block::setBlockFromFile(BlockNameType t, int o, BlockDataType* inBlockPtr, bool inCf) {

    std::map<BlockNameType, std::string> blocksToLoad;
    blocksToLoad.emplace(BlockNameType::cube, "a-");
    blocksToLoad.emplace(BlockNameType::tee, "b-");
    blocksToLoad.emplace(BlockNameType::rLee, "c-");
    blocksToLoad.emplace(BlockNameType::zee, "d-");
    blocksToLoad.emplace(BlockNameType::mzee, "e-");
    blocksToLoad.emplace(BlockNameType::lLee, "f-");
    blocksToLoad.emplace(BlockNameType::line, "g-");

    loadSingleOrientation(blocksToLoad[t] + std::to_string(o) + "-", inBlockPtr);
            mRect.h = bottomEdgeOfOrientation(inBlockPtr);
            mRect.w = rightEdgeOfOrientation(inBlockPtr);
            mCanRotate = canRotate(blocksToLoad[t], mBlockDataPtr);

    //This is for the clear block.
    if(inCf == 1) {
        (*inBlockPtr)[0].fill(BlockTexCode::O);
        (*inBlockPtr)[1].fill(BlockTexCode::O);
        (*inBlockPtr)[2].fill(BlockTexCode::O);
        (*inBlockPtr)[3].fill(BlockTexCode::O);
        (*inBlockPtr)[4].fill(BlockTexCode::O);
    }
}

//cube,tee,rLee,zee,mzee,lLee,line,empty
void ley::Block::setBlock(BlockNameType t, int o) {
    
    mOrientation = o;

    switch (t) {
        case BlockNameType::cube :
            switch(o)
            case 0 :
            case 1 :
            case 2 :
            case 3 :
                mBlockData[0] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[1] = { BlockTexCode::d, BlockTexCode::d, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mRect.h = 2;
                mRect.w = 2;
            break;
        break;
        case BlockNameType::tee :
            switch(o) {
                case 0 :
                    mBlockData[0] = { BlockTexCode::O, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::O, BlockTexCode::e, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h=3;
                    mRect.w=3;
                break;
                case 1 :
                    mBlockData[0] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::e, BlockTexCode::e, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h = 3;
                    mRect.w = 3;
                break;
                case 2 :
                    mBlockData[0] = { BlockTexCode::O, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::e, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h=3;
                    mRect.w=2;
                break;
                case 3 :
                    mBlockData[0] = { BlockTexCode::O, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::e, BlockTexCode::e, BlockTexCode::e, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h=2;
                    mRect.w=3;
                break;
            }
        break;
        case BlockNameType::rLee :
             switch(o) {
                case 0 :
                    mBlockData[0] = { BlockTexCode::f, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::f, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::f, BlockTexCode::f, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h = 3;
                    mRect.w = 2;
                break;
                case 1 :
                    mBlockData[0] = { BlockTexCode::f, BlockTexCode::f, BlockTexCode::f, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::f, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h = 2;
                    mRect.w = 3;
                break;
                case 2 :
                    mBlockData[0] = { BlockTexCode::O, BlockTexCode::f, BlockTexCode::f, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::f, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::f, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h = 3;
                    mRect.w = 3;
                break; 
                case 3 :
                    mBlockData[0] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::f, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::f, BlockTexCode::f, BlockTexCode::f, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h = 3;
                    mRect.w = 3;
                break;
             }
        break;
        case BlockNameType::zee :
            switch(o) {
                case 0 :
                case 2 :
                    mBlockData[0] = { BlockTexCode::O, BlockTexCode::g, BlockTexCode::g, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::g, BlockTexCode::g, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h = 2;
                    mRect.w = 3;
                break;
                case 1 :
                case 3 :
                    mBlockData[0] = { BlockTexCode::g, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::g, BlockTexCode::g, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::g, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h = 3;
                    mRect.w = 2;
                break;
            }
        break;
        case BlockNameType::mzee :
             switch(o) {
                case 0 :
                case 2 :
                    mBlockData[0] = { BlockTexCode::h, BlockTexCode::h, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::O, BlockTexCode::h, BlockTexCode::h, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mRect.h = 2;
                mRect.w = 3;
                break;
                case 1 :
                case 3 :
                    mBlockData[0] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::h, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::O, BlockTexCode::h, BlockTexCode::h, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::h, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mRect.h = 3;
                mRect.w = 3;
                break;
             }
        break;
        case BlockNameType::lLee :
            switch (o) {
            case 0 :
                mBlockData[0] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::i, BlockTexCode::O, BlockTexCode::O };
                mBlockData[1] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::i, BlockTexCode::O, BlockTexCode::O };
                mBlockData[2] = { BlockTexCode::O, BlockTexCode::i, BlockTexCode::i, BlockTexCode::O, BlockTexCode::O };
                mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mRect.h = 3;
                mRect.w = 3;
            break;
            case 1 :
                mBlockData[0] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[1] = { BlockTexCode::i, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[2] = { BlockTexCode::i, BlockTexCode::i, BlockTexCode::i, BlockTexCode::O, BlockTexCode::O };
                mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mRect.h = 3;
                mRect.w = 3;
            break;
            case 2 :
                mBlockData[0] = { BlockTexCode::i, BlockTexCode::i, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[1] = { BlockTexCode::i, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[2] = { BlockTexCode::i, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mRect.h = 3;
                mRect.w = 2;
            break;
            case 3 :
                mBlockData[0] = { BlockTexCode::i, BlockTexCode::i, BlockTexCode::i, BlockTexCode::O, BlockTexCode::O };
                mBlockData[1] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::i, BlockTexCode::O, BlockTexCode::O };
                mBlockData[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                mRect.h = 2;
                mRect.w = 3;
            break;
            }
        break;
        case BlockNameType::line :
            switch (o) {
                case 0 :
                case 2 :
                    mBlockData[0] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::j, BlockTexCode::j, BlockTexCode::j, BlockTexCode::j, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h = 2;
                    mRect.w = 4;
                break;

                case 1 :
                case 3 :
                    mBlockData[0] = { BlockTexCode::O, BlockTexCode::j, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[1] = { BlockTexCode::O, BlockTexCode::j, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[2] = { BlockTexCode::O, BlockTexCode::j, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[3] = { BlockTexCode::O, BlockTexCode::j, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mBlockData[4] = { BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O, BlockTexCode::O };
                    mRect.h = 4;
                    mRect.w = 2;
                break;
            }
        break;
        default : ;
    }

    if(mClearFlag == 1) {
        mBlockData[0].fill(BlockTexCode::O);
        mBlockData[1].fill(BlockTexCode::O);
        mBlockData[2].fill(BlockTexCode::O);
        mBlockData[3].fill(BlockTexCode::O);
        mBlockData[4].fill(BlockTexCode::O);
    }
}

Uint8 ley::Block::getLeftGap() {
    //Iterate through the entire block to discover the left gap.
    Uint8 smallestGap = mBlockData[0].size(); //Assume that the block is square and we will start with the width of the block based on the first row
    for(int i = 0; i < mBlockData.size(); ++i) {

        Uint8 rowGap = 0;
        for(int j = 0; j < mBlockData[i].size(); ++j) {

            if(mBlockData[i][j] != ley::BlockTexCode::O) {
                break;
            }
            else {
                rowGap++;
            }
        }

        //We want the largest gap that has a texture in it, if there is a row that has all gaps then we don't count that one.
        if(rowGap < smallestGap && rowGap < mBlockData[i].size()) {
            
            smallestGap = rowGap;
        }
    }

    return smallestGap;
}

// This will let us know how many gap rows are on top of the block.
Uint8 ley::Block::getTopGap() {
    //How many full rows on top do not have a texture.
    for(int i = 0; i < mBlockData.size(); ++i) {
        for(int j = 0; j < mBlockData[i].size(); ++j) {
            if(mBlockData[i][j] != ley::BlockTexCode::O) {
                return i;
            }
        }
    }
    return mBlockData.size();
}
