#include "../inc/Board.h"

void ley::Board::assign(int inX, int inY) {
    
    if(inX > 0 && inY > 0)
    {
        mWidth = inX;
        mHeight = inY;
        mBoard.assign(inX * inY, std::make_pair(ley::BlockTexCode::O, false));
    }
}

std::pair<ley::BlockTexCode, bool>* ley::Board::at(int inX, int inY) {
   
   return &mBoard.at(inY * mWidth + inX);
}

void ley::Board::debugOutput() {

    for(int i = 0; i < mHeight; ++i) {
        std::string s;
        for(int j = 0; j < mWidth; ++j) {
            s += TEXCODE_CHAR.find(at(j,i)->first)->second + ".";
            
        }
        SDL_Log(s.c_str());
    }
}