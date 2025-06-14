#include "../../inc/State/BlockEditorState.h"
 

typedef ley::Textures TextureManager;

namespace ley {

const std::string BlockEditorState::sBlockEditorID = "BLOCKEDITOR";

BlockEditorState::BlockEditorState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mTitleFont{20,20,100,50} {

    // TODO localization
    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("block editor comming soon", 0, false, capitalizationtype::capitalizeWords));

    updateBlockEditorFonts();

    mLayout.setLayout({50,50,30,30},0,0,5,5);
    mBlockUIMenu.setWidth(5);

    for(int i = 0; i < 25; ++i) {
        
        mTiles.push_back(std::make_unique<UI_Tile>());
        SDL_Rect layoutRect = mLayout.getRectForCell(i);

        mTiles.back()->setPos({layoutRect.x,layoutRect.y});
        mTiles.back()->setWidth(layoutRect.w, layoutRect.w, 2);
        mTiles.back()->setVisible(false);
        mTiles.back()->setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
        mTiles.back()->setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
        mTiles.back()->setRegEx("\\b(?:[8-9]|1\\d|2[0-5])x(?:[8-9]|1\\d|2[0-2])\\b");
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
}

void BlockEditorState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
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

}