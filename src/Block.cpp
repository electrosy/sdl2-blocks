/* 
sdl2-blocks
Copyright (C) 2020 Steven Philley

Purpose: see header.
Date: Feb/15/2020
*/
#include "../inc/Block.h"

ley::BlockFileDataMapType* ley::Block::mBlockDataPtr = nullptr;

namespace ley {

// Single authoritative definitions — declared extern in Block.h.
const std::unordered_map<BlockTexCode, std::string> TEXCODE_CHAR {
    {BlockTexCode::a, "a"}, {BlockTexCode::b, "b"}, {BlockTexCode::c, "c"},
    {BlockTexCode::d, "d"}, {BlockTexCode::e, "e"}, {BlockTexCode::f, "f"},
    {BlockTexCode::g, "g"}, {BlockTexCode::h, "h"}, {BlockTexCode::i, "i"},
    {BlockTexCode::j, "j"}, {BlockTexCode::k, "k"}, {BlockTexCode::l, "l"},
    {BlockTexCode::O, "O"},
};

const std::unordered_map<std::string, BlockTexCode> CHAR_TEXCODE {
    {"a", BlockTexCode::a}, {"b", BlockTexCode::b}, {"c", BlockTexCode::c},
    {"d", BlockTexCode::d}, {"e", BlockTexCode::e}, {"f", BlockTexCode::f},
    {"g", BlockTexCode::g}, {"h", BlockTexCode::h}, {"i", BlockTexCode::i},
    {"j", BlockTexCode::j}, {"k", BlockTexCode::k}, {"l", BlockTexCode::l},
    {"O", BlockTexCode::O},
};

} // namespace ley
/* RAII */
ley::Block::Block() {

}
ley::Block::Block(unsigned int boardWidth, unsigned int y, BlockNameType ty, bool c)
: mType(ty), mClearFlag(c) {
    
    mRect.y = y;
    mOrientation = 0;
    if(mReadBlockFromFile) {
        setBlockDataFromFile(mType, mOrientation, &mBlockData, mClearFlag, &mRect, &mCanRotate);
    }
    else {
        setBlock(mType);
    }
    recalcGaps();
    
    //Set the x position so that the block piece is placed into the center of the board width.
    mRect.x = boardWidth/2 - width()/2;
}

ley::Block::Block(const Block& b) {
    mType = b.mType;
    mOrientation = b.mOrientation; // 0-3 - rotating to the left piece points right,down,left,up
    mRect = b.mRect; //Position and dimension
    mClearFlag = b.mClearFlag;
    
    if(mReadBlockFromFile) {
        setBlockDataFromFile(b.mType, mOrientation, &mBlockData, mClearFlag, &mRect, &mCanRotate);
    }
    else {
        setBlock(b.mType);
    }
    recalcGaps();
}

void ley::Block::operator=(const Block& b) {
    mType = b.mType;
    mOrientation = b.mOrientation; // 0-3 - rotating to the left piece points right,down,left,up
    mRect = b.mRect; //Position and dimension
    mClearFlag = b.mClearFlag;
    if(mReadBlockFromFile) {
        setBlockDataFromFile(b.mType, mOrientation, &mBlockData, mClearFlag, &mRect, &mCanRotate);
    }
    else {
        setBlock(b.mType);
    }
    recalcGaps();

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
        setBlockDataFromFile(mType, 0, &mBlockData, mClearFlag, &mRect, &mCanRotate);
    }
    else {
        setBlock(mType);
    }
    recalcGaps();
}

void ley::Block::reset() {
    
    mRect.x = BLOCK_START_X;
    mRect.y = BLOCK_START_Y;
    mOrientation = 0;
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
        setBlockDataFromFile(mType,this->mOrientation, &mBlockData, mClearFlag, &mRect, &mCanRotate);
    }
    else {
        setBlock(mType,this->mOrientation);
    }
    recalcGaps();

    if(!getCanRotate()) {
        return false;
    }

    return true;
}

int ley::Block::height() const {
    return mCachedHeight;
}

int ley::Block::width() const {
    return mCachedWidth;
}


void ley::Block::setBlockDataPtr(BlockFileDataMapType* blockDataPtr) {
    mBlockDataPtr = blockDataPtr;
}

void ley::Block::loadSingleOrientation(std::string orientation, BlockDataType* blockData, BlockFileDataMapType* inBlockDataFileMapPtr) {

    auto blockTexCodeFromString = [](char str_code) -> ley::BlockTexCode {
        static const std::unordered_map<char, ley::BlockTexCode> strTexCodeMap {
            {'O', ley::BlockTexCode::O}, {'a', ley::BlockTexCode::a},
            {'b', ley::BlockTexCode::b}, {'c', ley::BlockTexCode::c},
            {'d', ley::BlockTexCode::d}, {'e', ley::BlockTexCode::e},
            {'f', ley::BlockTexCode::f}, {'g', ley::BlockTexCode::g},
            {'h', ley::BlockTexCode::h}, {'i', ley::BlockTexCode::i},
            {'j', ley::BlockTexCode::j}, {'k', ley::BlockTexCode::k},
            {'l', ley::BlockTexCode::l}, {'Z', ley::BlockTexCode::Z},
        };
        auto it = strTexCodeMap.find(str_code);
        return it != strTexCodeMap.end() ? it->second : ley::BlockTexCode::O;
    };

    // TODO BLOCK_SIZE is assumed to be square, maybe we should define a height and width.
    for(int i = 0; i < BLOCK_SIZE; ++i) {

        std::string str_data = (*inBlockDataFileMapPtr)[orientation + std::to_string(i)];
        
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


bool ley::Block::hasMoreThanOneOrientation(std::string blockCharName, BlockFileDataMapType* inBlockDataFileMapPtr) {

    return (*inBlockDataFileMapPtr)[blockCharName + "*"] == "yes" ? true : false;
}

void ley::Block::setBlockDataFromFile(BlockNameType t, int o, BlockDataType* inBlockPtr, bool inCf, SDL_Rect* inRectPtr, bool* inCanRotatePtr) {

    static const std::unordered_map<BlockNameType, std::string> blocksToLoad {
        {BlockNameType::cube, "a-"}, {BlockNameType::tee,  "b-"},
        {BlockNameType::rLee, "c-"}, {BlockNameType::zee,  "d-"},
        {BlockNameType::mzee, "e-"}, {BlockNameType::lLee, "f-"},
        {BlockNameType::line, "g-"},
    };

    loadSingleOrientation(blocksToLoad.at(t) + std::to_string(o) + "-", inBlockPtr, mBlockDataPtr);
            inRectPtr->h = bottomEdgeOfOrientation(inBlockPtr);
            inRectPtr->w = rightEdgeOfOrientation(inBlockPtr);
            if(inCanRotatePtr) {
                (*inCanRotatePtr) = hasMoreThanOneOrientation(blocksToLoad.at(t), mBlockDataPtr);
            }

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

    recalcGaps();
}

// Recalculate and cache all geometry values. Called after any mBlockData mutation.
void ley::Block::recalcGaps() {

    // Left gap: smallest number of leading-empty columns across all rows that contain a tile
    Uint8 smallestGap = static_cast<Uint8>(mBlockData[0].size());
    for(int i = 0; i < static_cast<int>(mBlockData.size()); ++i) {
        Uint8 rowGap = 0;
        for(int j = 0; j < static_cast<int>(mBlockData[i].size()); ++j) {
            if(mBlockData[i][j] != ley::BlockTexCode::O) break;
            else ++rowGap;
        }
        // Only count rows that actually contain a tile
        if(rowGap < smallestGap && rowGap < static_cast<Uint8>(mBlockData[i].size())) {
            smallestGap = rowGap;
        }
    }
    mCachedLeftGap = smallestGap;

    // Top gap: number of fully-empty rows at the top
    mCachedTopGap = static_cast<Uint8>(mBlockData.size()); // default: all rows empty
    for(int i = 0; i < static_cast<int>(mBlockData.size()); ++i) {
        for(int j = 0; j < static_cast<int>(mBlockData[i].size()); ++j) {
            if(mBlockData[i][j] != ley::BlockTexCode::O) {
                mCachedTopGap = static_cast<Uint8>(i);
                goto top_gap_done; // break out of both loops
            }
        }
    }
    top_gap_done:;

    // Height: number of rows from first to last tile, including interior gaps
    {
        int h = 0, gap = 0;
        bool counting = false;
        for (int i = 0; i < static_cast<int>(mBlockData.size()); ++i) {
            bool rowOccupied = false;
            for (int k = 0; k < static_cast<int>(mBlockData[i].size()); ++k) {
                if (mBlockData[i][k] != ley::BlockTexCode::O) { rowOccupied = true; break; }
            }
            if (rowOccupied) {
                h += 1 + gap;
                gap = 0;
                counting = true;
            } else if (counting) {
                ++gap;
            }
        }
        mCachedHeight = h;
    }

    // Width: number of columns from first to last tile, including interior gaps
    {
        int w = 0, gap = 0;
        bool counting = false;
        for (int i = 0; i < static_cast<int>(mBlockData[0].size()); ++i) {
            bool colOccupied = false;
            for (int k = 0; k < static_cast<int>(mBlockData.size()); ++k) {
                if (mBlockData[k][i] != ley::BlockTexCode::O) { colOccupied = true; break; }
            }
            if (colOccupied) {
                w += 1 + gap;
                gap = 0;
                counting = true;
            } else if (counting) {
                ++gap;
            }
        }
        mCachedWidth = w;
    }
}

Uint8 ley::Block::getLeftGap() const { return mCachedLeftGap; }
Uint8 ley::Block::getTopGap()  const { return mCachedTopGap;  }
