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
#include <unordered_map>
#include <SDL2/SDL.h>

#include "Rand_int.h"

namespace ley {

// Block geometry constants — placed in namespace ley to avoid polluting global scope
constexpr int   BLOCK_START_X        = 4;
constexpr int   BLOCK_START_Y        = 0;
constexpr int   BLOCK_SIZE           = 5; // Maximum block width and height
constexpr Uint8 MAX_ORIENTATIONS     = 4;
constexpr Uint8 NUM_DIFFERENT_BLOCKS = 7;

enum class BlockNameType {cube,tee,rLee,zee,mzee,lLee,line,empty};
// the letter 'O' means empty and is the default. Capital 'Z' means don't clear part if clear block.
enum class BlockTexCode {a,b,c,d,e,f,g,h,i,j,k,l,O,Z};

typedef std::unordered_map<std::string, std::string> BlockFileDataMapType;
typedef std::array<std::array<BlockTexCode, BLOCK_SIZE>,BLOCK_SIZE> BlockDataType;
typedef std::pair<ley::BlockTexCode, bool> BoardCellType;

// Defined once in Block.cpp; all TUs share the single construction.
extern const std::unordered_map<BlockTexCode, std::string> TEXCODE_CHAR;
extern const std::unordered_map<std::string, BlockTexCode> CHAR_TEXCODE;

} // namespace ley

// std::hash specializations so enum class values can be unordered_map keys
namespace std {
    template<> struct hash<ley::BlockTexCode> {
        size_t operator()(ley::BlockTexCode v) const noexcept {
            return std::hash<int>{}(static_cast<int>(v));
        }
    };
    template<> struct hash<ley::BlockNameType> {
        size_t operator()(ley::BlockNameType v) const noexcept {
            return std::hash<int>{}(static_cast<int>(v));
        }
    };
}

namespace ley {

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

    // Geometry cache — recalculated once whenever mBlockData changes
    Uint8 mCachedLeftGap   = 0;
    Uint8 mCachedTopGap    = 0;
    int   mCachedHeight    = 0;
    int   mCachedWidth     = 0;
    void recalcGaps();

protected:
    static BlockFileDataMapType* mBlockDataPtr;

public:
    /* RAII */
    Block();
    Block(unsigned int, unsigned int, BlockNameType, bool = 0); //boardwidth,y,type,clear
    Block(const Block& b); //copy constructor
    ~Block();
    /* Operators */
    void operator= (const Block &b);
    /* Functions */
    static void loadSingleOrientation(std::string orientation, BlockDataType* blockData, BlockFileDataMapType* inBlockDataFileMapPtr);
    static int bottomEdgeOfOrientation(BlockDataType* blockData);
    static int rightEdgeOfOrientation(BlockDataType* blockData);
    BlockTexCode renderPart(unsigned int, unsigned int); // Return true or false depending on if the block would take up the x,y space
    void moveDown();
    void moveLeft();
    void moveRight();
    int height() const; // return the actual height of the block (cached)
    int width()  const; // return the actual width of the block (cached)
    int heightAtWidth(int width); //return the number of blocks high at a particular x location
    int widthAtHeight(int); //return the number of blocks wide at a particular y location
    void reset(); //return block to original position for restarting the game.
    Uint8 getLeftGap() const;
    Uint8 getTopGap()  const;
    SDL_Rect* getPosRectPtr() { return &mRect;};
    static void setBlockDataPtr(BlockFileDataMapType* blockDataPtr);
    bool getCanRotate(){ return mCanRotate;};
    static bool hasMoreThanOneOrientation(std::string blockCharName, BlockFileDataMapType* inBlockDataFileMapPtr);
    /* Accessors */
    SDL_Rect getRect() const {return mRect;};
    const BlockNameType getType() {return mType;};
    const bool getClear() {return mClearFlag;};
    void setClear(bool c);
    void setH(unsigned int h) {mRect.h = h;};
    void setW(unsigned int w) {mRect.w = w;};
    void debugResetPos();
    bool rotate(bool); // input false for counterclockwise and true for clockwise. return true if there is more than one orientation
    std::array<std::array<ley::BlockTexCode, BLOCK_SIZE>,BLOCK_SIZE> getBlockParts();
    static void setBlockDataFromFile(BlockNameType t, int o, BlockDataType* inBlockPtr, bool inCf, SDL_Rect* inRectPtr, bool* inCanRotatePtr);
    BlockDataType* getBlockDataPtr() {return &mBlockData;};
    unsigned int getBlockOrientation() {return mOrientation;};
};

} 
#endif
