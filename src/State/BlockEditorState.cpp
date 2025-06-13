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

//    mBlockEditorUI.p

    updateBlockEditorFonts();

    mActiveUIElement = {};
    
    mFirstTile.setVisible(false);
    mFirstTile.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mFirstTile.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mFirstTile.setWidthByChar(5);
    mFirstTile.setRegEx("\\b(?:[8-9]|1\\d|2[0-5])x(?:[8-9]|1\\d|2[0-2])\\b");
    mFirstTile.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst) + "," 
        + mGameModel->getLanguageModel()->getWord("e.g. 10x20", 0, false, capitalizationtype::capitalizeNone)
        , "");
    mFirstTile.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be two numbers seperated by an 'x' between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst));
    mFirstTile.setPos({224,100});


    /*
    mBlockUIMenu.pushUIElement(
        [this](){mFirstTile.handleFocusChange({}, &mPreviousOptionsValue);},
        [this]()->bool{return mFirstTile.hasFocus();},
        [this](){ commitTile(); } );
    */

}

void BlockEditorState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    mBlockUIMenu.runCommand(command);
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

    mRenderables.push_back(&mFirstTile);
}

bool BlockEditorState::onEnter() {

    SDL_Log("Entering BlockEditorState");
    loadRenderables();

    mFirstTile.setVisible(true);

    
    return true;
}

bool BlockEditorState::onReEnter() {
    SDL_Log("ReEntering BlockEditorState");
    return true;
}

bool BlockEditorState::onExit() {
    SDL_Log("Exiting BlockEditorState");
    return true;
}

bool BlockEditorState::onPause() {
    GameState::onPause();
    
    return true;
}

void BlockEditorState::updateBlockEditorFonts() {

    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("block editor coming soon", 0, false, capitalizationtype::capitalizeWords));
}

void BlockEditorState::commitTile() {
    
}

}