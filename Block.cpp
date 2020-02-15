/* 
Author: Steven Philley
Purpose: see header.
Date: Feb/15/2020
*/
#include "Block.h"

/* RAII */
ley::Block::Block(ley::Sprite& s, bool a, std::vector<SDL_Rect>* v) {
    animated = a;
    if(animated == true; v != nullptr) { //there must be some frames available.
       for(auto rect : *v) {

           
            // TODO create the sprite/frame
            // Push the sprite onto the vector.    
       }
    } else { //this is a single frame block
        frames.push_back(&s);
    }
}

ley::Block::~Block() {

}

/* Functions */
ley::Sprite* ley::Block::getFrame(int num) {
    return frames[num];
}