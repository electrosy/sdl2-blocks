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
const int BLOCK_SIZE = 5; //Maximum size of the block width and height.
const Uint8 MAX_ORIENTATIONS = 4;
const Uint8 NUM_DIFFERENT_BLOCKS = 7;

namespace ley {

enum class BlockNameType {cube,tee,rLee,zee,mzee,lLee,line,empty};
// the letter 'O' means empty and is the default. Capital 'Z' means don't clear part if clear block.
enum class BlockTexCode {a,b,c,d,e,f,g,h,i,j,k,l,O,Z};

typedef std::map<std::string, std::string> BlockFileDataMapType;
typedef std::array<std::array<BlockTexCode, BLOCK_SIZE>,BLOCK_SIZE> BlockDataType;
typedef std::pair<ley::BlockTexCode, bool> BoardCellType;

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

const std::map<std::string, BlockTexCode> CHAR_TEXCODE {
    std::make_pair ("a", BlockTexCode::a), // 12 texture possibilities
    std::make_pair ("b", BlockTexCode::b), //
    std::make_pair ("c", BlockTexCode::c), //
    std::make_pair ("d", BlockTexCode::d), //
    std::make_pair ("e" ,BlockTexCode::e), //
    std::make_pair ("f" ,BlockTexCode::f), //
    std::make_pair ("g" ,BlockTexCode::g), //
    std::make_pair ("h" ,BlockTexCode::h), //
    std::make_pair ("i" ,BlockTexCode::i), //
    std::make_pair ("j" ,BlockTexCode::j), //
    std::make_pair ("k" ,BlockTexCode::k), //
    std::make_pair ("l" ,BlockTexCode::l), //
    std::make_pair ("O" ,BlockTexCode::O), //This is the empty texture blockpart.
  };

class Block {

private:
    bool mClearFlag; //clear flag, used for a clear block, to clean up the oldposition.
    bool mReadBlockFromFile = 1;
    bool mCanRotate = false;
    unsigned int mOrientation; // 0-3 - rotating to the left piece points right,down,left,up
    SDL_Rect mRect; //Position and max dimension
    BlockNameType mType; //type empty == null block
    BlockDataType mBlockData;
    void setBlock(BlockNameType,int = 0);

protected:
    static BlockFileDataMapType* mBlockDataPtr;

public:
    /* RAII */
    Block();
    Block(unsigned int, unsigned int, BlockNameType, bool = 0); //x,y,type,clear
    Block(const Block& b); //copy constructor
    ~Block();
    static void loadSingleOrientation(std::string orientation, BlockDataType* blockData, BlockFileDataMapType* inBlockDataFileMapPtr);
    static int bottomEdgeOfOrientation(BlockDataType* blockData);
    static int rightEdgeOfOrientation(BlockDataType* blockData);
    /* Accessors */
    SDL_Rect getRect() {return mRect;}; // TODO this should probably return a const
    const BlockNameType getType() {return mType;};
    const bool getClear() {return mClearFlag;};
    void setClear(bool c);
    void setH(unsigned int h) {mRect.h = h;};
    void setW(unsigned int w) {mRect.w = w;};
    void debugResetPos();
    bool rotate(bool); // input false for counterclockwise and true for clockwise. return true if there is more than one orientation
    std::array<std::array<ley::BlockTexCode, BLOCK_SIZE>,BLOCK_SIZE> getBlockParts();

    void operator= (const Block &b);
    
    /* Functions */
    BlockTexCode renderPart(unsigned int, unsigned int); // Return true or false depending on if the block would take up the x,y space
    void moveDown();
    void moveLeft();
    void moveRight();
    int height(); // return the actual height of the block
    int width(); // return the actual width of the block
    int heightAtWidth(int width); //return the number of blocks high at a particular x location
    int widthAtHeight(int); //return the number of blocks wide at a particular y location
    void reset(); //return block to original position for restarting the game.
    Uint8 getLeftGap();
    Uint8 getTopGap();
    SDL_Rect* getPosRectPtr() { return &mRect;};
    static void setBlockDataPtr(BlockFileDataMapType* blockDataPtr);
    bool getCanRotate(){ return mCanRotate;};
    static bool canRotate(std::string blockCharName, BlockFileDataMapType* inBlockDataFileMapPtr);
    static void setBlockDataFromFile(BlockNameType t, int o, BlockDataType* inBlockPtr, bool inCf, SDL_Rect* inRectPtr, bool* inCanRotatePtr);
    BlockDataType* getBlockDataPtr() {return &mBlockData;};
    unsigned int getBlockOrientation() {return mOrientation;};
};

} 
#endif
