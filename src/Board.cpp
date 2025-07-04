#include "../inc/Board.h"

typedef ley::Textures TextureManager;

ley::Board::Board()
:
mBlockWidthPx(BLOCKSIZE_PX),
mBlockHeightPx(BLOCKSIZE_PX),
mBlockStartY(BLOCK_START_POS_Y_PX) {

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
}

std::pair<ley::BlockTexCode, bool>* ley::Board::at(int inX, int inY) {
    
    int value = inY * mWidth + inX;

    if(value >= 0 && value < mBoard.size()) {
        return &mBoard.at(inY * mWidth + inX);
    }
    else
        return {};

   
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

std::pair<bool, std::string> ley::Board::canPut(Block& b, Command d) {

    SDL_Rect block = b.getRect();
    // TODO these 4 nested loops can probably be combined into one as there are only minor differences.
    //Iterate through the block and check and see if the board is empty where we need to put a block part.
    //Check if there is already a block along the edge of the existing block.
    switch (d) {
        case Command::down : 
           for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) {
                    
                    bool renderPart = b.renderPart(i, j) != BlockTexCode::O; // part to render
                    std::pair<ley::BlockTexCode, bool>* boardAt = at(block.x + i, block.y + j + 1);
                    bool boardPart = block.y + j + 1 >= mHeight
                                        || (boardAt ? boardAt->second : false); //the space is already occupied
                    if(renderPart && boardPart) {
                            return {false, "test"}; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        case Command::right : 
            for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) {
                    std::pair<ley::BlockTexCode, bool>* boardAt = at(block.x + i + 1, block.y + j);
                    bool boardPart = ((block.x + 1) + i) > (mWidth - 1) || (boardAt ? boardAt->second : false);
                                    
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                           return {false, "test"}; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        case Command::left :
            for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) { 
                    std::pair<ley::BlockTexCode, bool>* boardAt = at((block.x - 1) + i, block.y + j);
                    bool boardPart = ((block.x - 1) + i) < 0
                                     || (boardAt ? boardAt->second : false);
                                    
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart == true)//the space is already occupied
                    ) {
                            return {false, "test"}; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        // TODO will rotation actually work for all cases??
        case Command::up : //this is a rotation
            for(int i = 0; i < block.w; ++i) {
               for(int j = 0; j < block.h; ++j) {
                    //if there is a block piece to put,then check to see if it can be put.
                    bool boardBottomPart =  //board empty.
                                    // TODO this logic is weird, why mHeight and mHeight -1?
                                    block.y + j > mHeight - 1 || block.y + j > mHeight; //below the board
                    bool boardPart = block.x + i > mWidth - 1 //right of the board
                                    || block.x + i < 0; // left of the board
                    std::pair<ley::BlockTexCode, bool>* boardAt = at(block.x + i, block.y + j);
                    bool blockCollide = boardAt ? boardAt->second : false;
                    if( (b.renderPart(i, j) != BlockTexCode::O)//we have a part to render.
                        && (boardPart || blockCollide || boardBottomPart)//the space is already occupied
                    ) {
                            // TODO this double ternary is a little hard to read.
                            return {false, boardBottomPart ? "board_bottom" : boardPart ? "board" : "block"}; /*** EARLY EXIT! ***/
                    }
               }
           }
        break;
        default : break;
    }

    return {true, "test"};
}
void ley::Board::putBlock(Block& b) {
    SDL_Rect rect = b.getRect();
    for(int i = 0; i < rect.w; ++i) {
        for(int j = 0; j < rect.h; ++j) {
            BlockTexCode renderPart = b.renderPart(i, j);
            if(at(rect.x+i, rect.y+j) && at(rect.x+i, rect.y+j)->second == false) {
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
                // TODO add the check here for board out of bounds
                BoardCellType* boardAt;
                boardAt = at(blockRect.x + i, blockRect.y + j);
                if(boardAt) {
                    boardAt->second = true;
                }
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