/* 
Author: Steven Philley
Purpose: A Block is a collection of sprites constructed together 
         to make a larger sprite. It can also be a single sprite. 
         A block contains the physical bounderies for collision.
Date: Feb/15/2020
*/
#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

#include "Sprite.h"


namespace ley {

class Block {

private:
    bool animated = 0;
    std::vector<Sprite*>frames;

public:
    Block(Sprite&, bool = 0, std::vector<SDL_Rect>* = nullptr);
    ~Block();

    Sprite* getFrame(int = 0);
};

}
#endif
