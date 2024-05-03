#include "../inc/Board.h"

typedef ley::Textures TextureManager;

ley::Board::Board()
:
mBlockWidthPx(BLOCKSIZE_PX),
mBlockHeightPx(BLOCKSIZE_PX),
mBlockStartY(BLOCK_START_POS_Y_PX) {

//setSize(BOARDSIZE_WIDTH,BOARDSIZE_HEIGHT);

}

void ley::Board::setSize(int inX, int inY) {
    
    if(inX > 0 && inY > 0)
    {
        mWidth = inX;
        mHeight = inY;
        mWidthPx = mWidth * mBlockWidthPx;
        mHeightPx = mHeight * mBlockHeightPx;
        mBoardPosXPx = (SCREEN_WIDTH / 2) - (mWidthPx / 2);
        mNextBoxPosXPx = mBoardPosXPx - NEXTBOX_PADDING_PX - NEXTBOX_SIZE_PX;
        mScorePosXPx = mBoardPosXPx + mWidthPx + NEXTBOX_PADDING_PX;
        mBoard.assign(inX * inY, std::make_pair(ley::BlockTexCode::O, false));
    }

//    fillLine(21,std::make_pair(ley::BlockTexCode::d, false));
//    shiftBoard(21,1);

/* //testing code
    at(5,7)->first = ley::BlockTexCode::d;
    at(5,7)->second = true;

    at(4,9)->first = ley::BlockTexCode::d;
    at(4,9)->second = true;

    at(9,9)->first = ley::BlockTexCode::e;
    at(9,9)->second = true;

    at(9,19)->first = ley::BlockTexCode::e;
    at(9,19)->second = true;

    at(0,0)->first = ley::BlockTexCode::f;
    at(0,0)->second = true;
*/
}

std::pair<ley::BlockTexCode, bool>* ley::Board::at(int inX, int inY) {
   
   return &mBoard.at(inY * mWidth + inX);
}

void ley::Board::debugOutput(bool inLayer) {

    for(int i = 0; i < mHeight; ++i) {
        std::string s;
        for(int j = 0; j < mWidth; ++j) {

            if(inLayer) {
                s += std::to_string(at(j,i)->second) + ".";
            }
            else {

                s += TEXCODE_CHAR.find(at(j,i)->first)->second + ".";
            }            
        }
        SDL_Log(s.c_str());
    }
}

void ley::Board::render(SDL_Renderer * r, bool d) {
    //get width and height of the texture // TODO this should be dynamic based on image passed in
    int w = mBlockWidthPx, h = mBlockHeightPx; //SDL_QueryTexture(t, NULL, NULL, &w, &h);
    SDL_Rect start_rect{0,0,w,h};
    SDL_Rect dest_rect {mBoardPosXPx, mBlockStartY, w, h};
    SDL_Texture *blockBit = nullptr;

    for(int i = 0; i < mHeight; ++i) {
        for(int j = 0; j < mWidth; ++j) {
            if (at(j,i)->first != BlockTexCode::O) { 
                blockBit = TextureManager::Instance()->getTexture(TEXCODE_CHAR.at(at(j,i)->first));
                SDL_RenderCopy(r, blockBit, &start_rect, &dest_rect);
            }
            dest_rect.x = dest_rect.x + w;
        }
        dest_rect.y = dest_rect.y + h;
        dest_rect.x = mBoardPosXPx;
    }

}

void ley::Board::clear() {
    mBoard.clear();
    setSize(mWidth, mHeight);
}

bool ley::Board::canPut(Block& b, Command d) {

    SDL_Rect block = b.getRect();

    // TODO these 4 nested loops can probably be combined into one as there are only minor differences.
    //Iterate through the block and check and see if the board is empty where we need to put a block part.
    //Check if there is already a block along the edge of the existing block.
    switch (d) {
        case Command::down : 
           for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) {
                    
                    bool renderPart = b.renderPart(i, j) != BlockTexCode::O; // part to render
                    bool boardPart = block.y + j + 1 >= mHeight
                                        || at(block.x + i, block.y + j + 1)->second; //the space is already occupied
                    if(renderPart && boardPart) {
                            return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        case Command::right : 
            for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) {
                    bool boardPart = ((block.x + 1) + i) > (mWidth - 1)
                                        || at(block.x + i + 1, block.y + j)->second;
                                    
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                           return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        case Command::left :
            for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) { 
                    bool boardPart = ((block.x - 1) + i) < 0
                                     || at((block.x - 1) + i, block.y + j)->second;
                                    
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                            return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        case Command::up : //this is a rotation
            for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) {
                    //if there is a block piece to put,then check to see if it can be put.
                    bool boardPart =  //board empty.
                                    block.y + j > mHeight - 1 || block.y + j > mHeight //above the board
                                    || block.x + i > mWidth - 1 //right of the board
                                    || block.x + i < 0 // left of the board
                                    || at(block.x + i, block.y + j)->second;
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                            return false; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        default : break;
    }

    return true;
}
void ley::Board::putBlock(Block& b) {
    SDL_Rect rect = b.getRect();
    for(int i = 0; i < rect.w; ++i) {
        for(int j = 0; j < rect.h; ++j) {
            BlockTexCode renderPart = b.renderPart(i,j);
            if(at(rect.x+i, rect.y+j)->second == false) {
                at(rect.x+i, rect.y+j)->first = renderPart;
            }
        }
    }
}

void ley::Board::setBlock(Block& b) {
    SDL_Rect blockRect = b.getRect();
        for(auto i=0; i<blockRect.w; ++i) {
            for(auto j=0; j<blockRect.h; ++j) {
                if(b.renderPart(i,j) != BlockTexCode::O) {
                    at(blockRect.x + i, blockRect.y + j)->second = true;
                }
            }
        }
}

void ley::Board::fillLine(int l, std::pair<ley::BlockTexCode, bool> p) {

    if(l>height()-1) {
        SDL_Log("Tring to debug fill the board beyod bounds.");
        return;
    }

    for(int i = 0; i < mWidth; ++i) {
        at(i,l)->first = p.first;
        at(i,l)->second = p.second;
    }
}

void ley::Board::debugFill() {

    //TODO this need to take account of the actual board height because if we fill a row that doesn't exist we get a crash.
    fillLine(20, std::make_pair(ley::BlockTexCode::e, true));
    fillLine(21, std::make_pair(ley::BlockTexCode::e, true));
    fillLine(22, std::make_pair(ley::BlockTexCode::e, true));

    *at(0,20) = std::make_pair(ley::BlockTexCode::O, false);
    *at(1,20) = std::make_pair(ley::BlockTexCode::O, false);

    *at(0,21) = std::make_pair(ley::BlockTexCode::O, false);
    
    *at(0,22) = std::make_pair(ley::BlockTexCode::O, false);
    *at(1,22) = std::make_pair(ley::BlockTexCode::O, false);
}

/* TODO left off here, this needs work
void ley::Board::shiftBoard(char start, char num) {
    //shift the board down start from the bottom right (backwards).
    char stopLine = start - num;
    for(char i = start; i-num >= 0; --i) {
        for(char j = mWidth - 1; j >= 0; --j) {
            at(i,j)->first = at(i-num,j)->first;
            at(i,j)->second = at(i-num,j)->second;
        }
    }
}
*/