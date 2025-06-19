#include "../../inc/State/BlockEditorState.h"
 

typedef ley::Textures TextureManager;

namespace ley {

const std::string BlockEditorState::sBlockEditorID = "BLOCKEDITOR";

BlockEditorState::BlockEditorState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mTitleFont{0,0,100,50} {

    // TODO localization
    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("block editor comming soon", 0, false, capitalizationtype::capitalizeWords));

    updateBlockEditorFonts();

    int boardWidth = 35;
    int boardHeight = 20;
    mLayout.setLayout({20,20,30,30},0,0,boardWidth,boardHeight);
    mLayout.setMajorGrid(5, 10, 10);
    mBlockUIMenu.setWidth(boardWidth);

    int numCells = boardWidth * boardHeight;
    for(int i = 0; i < numCells; ++i) {
        
        mTiles.push_back(std::make_unique<UI_Tile>());
        SDL_Rect layoutRect = mLayout.getRectForCell(i);

        mTiles.back()->setPos({layoutRect.x,layoutRect.y});
        mTiles.back()->setWidth(layoutRect.w, layoutRect.w, 2);
        mTiles.back()->setVisible(false);
        mTiles.back()->setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
        mTiles.back()->setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
        mTiles.back()->setRegEx("\\b[defghij]\\b");
        mTiles.back()->setHelpMessages(mGameModel->getLanguageModel()->getWord("block editor tile " + std::to_string(mBlockUIMenu.rowAt(i) + 1) + "," + std::to_string(mBlockUIMenu.columnAt(i) + 1), 0, false, capitalizationtype::capitalizeFirst), "");
        mTiles.back()->setErrorMessage(mGameModel->getLanguageModel()->getWord("block editor tile. " + std::to_string(mBlockUIMenu.rowAt(i) + 1) + "," + std::to_string(mBlockUIMenu.columnAt(i) + 1), 0, false, capitalizationtype::capitalizeFirst));
    }

    mActiveUIElement = mTiles.back().get();
        
    for(const std::unique_ptr<UI_Tile> &tile : mTiles) {
        mBlockUIMenu.pushUIElement(
            [this,&tile](){tile->handleFocusChange(&mActiveUIElement, tile->getPreviousValuePtr());},
            [this,&tile]()->bool{return tile->hasFocus();},
            [this,&tile]() {tile->commit();}
        );   
    }

    loadBlocksKey();
    loadFromBlockDataPtr(mGameModel->getBlockDataPtr(), &mBlockData);
}

void BlockEditorState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    mSelectedTextureChar = mTiles[0]->getLastChar(); //pull the last char from the tiles widgets
    
    //set all the key borders to hidden
    for(auto& entry : mBlocksKeyRects) {
        entry.second.first = false;
    }

    //set the selected box to true.
    if(mBlocksKeyRects.find(mSelectedTextureChar) != mBlocksKeyRects.end()) {
        mBlocksKeyRects[mSelectedTextureChar].first = true;
    }

    mBlockUIMenu.runCommand(command);

    for(const std::unique_ptr<UI_Tile> &tile : mTiles) {
        dynamic_cast<UIWidget*>( tile.get() )->update();
    }

}

void BlockEditorState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }

    //render the highlight rects.
    for(std::pair<const std::string, std::pair<bool, SDL_Rect>>& rect_pair : mBlocksKeyRects) {
        const std::string& key = rect_pair.first; // Key (std::string)
        std::pair<bool, SDL_Rect>& value = rect_pair.second; // Value (std::pair<bool, SDL_Rect>)

        SDL_SetRenderDrawColor(mVideoSystem->getRenderer(), CWHITE.r, CWHITE.g, CWHITE.b, CWHITE.a);
        if(value.first) {
            SDL_RenderDrawRect(mVideoSystem->getRenderer(), &value.second);
        }
    }
    
    mBlockUIMenu.render(mVideoSystem);
}

void BlockEditorState::loadRenderables() {
    mRenderables.push_back(&mTitleFont);

    for(const std::unique_ptr<UI_Tile> &tile : mTiles) {
        mRenderables.push_back( dynamic_cast<UIWidget*>( tile.get() ));
    }
}

bool BlockEditorState::onEnter() {

    SDL_Log("Entering BlockEditorState");
    loadRenderables();

    for(const std::unique_ptr<UI_Tile> &tile : mTiles) {
        dynamic_cast<UIWidget*>( tile.get() )->setVisible(true);
    }

    
    return true;
}

bool BlockEditorState::onReEnter() {
    SDL_Log("ReEntering BlockEditorState");
    return true;
}

bool BlockEditorState::onExit() {
    SDL_Log("Exiting BlockEditorState");

    mActiveUIElement = {};

    return true;
}

bool BlockEditorState::onPause() {
    GameState::onPause();
    
    return true;
}

void BlockEditorState::updateBlockEditorFonts() {

    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("block editor coming soon", 0, false, capitalizationtype::capitalizeWords));
}

void BlockEditorState::loadBlocksKey() {

    std::vector<std::string> blocksToCheck;
    blocksToCheck.push_back("d");
    blocksToCheck.push_back("e");
    blocksToCheck.push_back("f");
    blocksToCheck.push_back("g");
    blocksToCheck.push_back("h");
    blocksToCheck.push_back("i");
    blocksToCheck.push_back("j");

    mKeyLayout.setLayout({375,650,0,0}, 100,0,7,1);

    SDL_Rect start_rect;
    start_rect.x = 0;
    start_rect.y = 0;
    start_rect.w = BLOCKSIZE_PX;
    start_rect.h = BLOCKSIZE_PX;

    mKeyLayout.resetIndex();
    for(std::string blockStr : blocksToCheck) {
        mBlockKeyFonts.push_back(ley::Font());
        mBlockKeyFonts.back().updateMessage(blockStr);
        SDL_Rect fontLayoutRect = mKeyLayout.getNextRect();
        mBlockKeyFonts.back().setPos({fontLayoutRect.x - 25, fontLayoutRect.y});
        mBlocksKeyRects.emplace( blockStr,  std::make_pair(false, (SDL_Rect){fontLayoutRect.x - 37, fontLayoutRect.y - 10, 90, 50})  );
    }

    for(int i = 0; i < blocksToCheck.size(); ++i) {
        //spriteBackground = ley::Sprite(TextureManager::Instance()->getTexture(background_level.c_str()), 0, {start_rect}, {500,{0,0,0,0}});
        mBlockKeySprites.push_back(ley::Sprite(TextureManager::Instance()->getTexture(blocksToCheck[i]), 0, {start_rect}, {10,{0,0,0,0}}));
        SDL_Rect blockKeyPosition = mKeyLayout.getRectForCell(i);
        mBlockKeySprites.back().setPos(blockKeyPosition.x, blockKeyPosition.y);
    }

    //Load the block sprites into the renderables.
    for(ley::Sprite& sprite : mBlockKeySprites) {
        mRenderables.push_back(&sprite);
    }

    //Load the fonts into the renderables.
    for(ley::Font& font : mBlockKeyFonts) {
        mRenderables.push_back(&font);
    }
}

void BlockEditorState::loadFromBlockDataPtr(BlockFileDataMapType* blockDataMapPtr, BlockDataType* blockDataTypePtr) {

    SDL_Rect rect;
    char o = '0';
    bool canRotate;
    BlockNameType blockType = BlockNameType::cube;

    Block::setBlockDataFromFile(blockType,0,blockDataTypePtr, false, &rect, &canRotate);
    transferBlockToTiles(blockDataTypePtr);
}

void BlockEditorState::transferBlockToTiles(BlockDataType* inBlockPtr) {

    for(std::array<BlockTexCode, BLOCK_SIZE>& blockRow : (*inBlockPtr) ) {
        blockRow;
        SDL_Log("Blowrow");
    }

}

}

