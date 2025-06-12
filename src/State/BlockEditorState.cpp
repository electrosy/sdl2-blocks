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

    mActiveUIElement = {};
}

void BlockEditorState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    mBlockEditorUI.runCommand(command);
}

void BlockEditorState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }

    mBlockEditorUI.render(mVideoSystem);
}

void BlockEditorState::loadRenderables() {
    mRenderables.push_back(&mTitleFont);
}

bool BlockEditorState::onEnter() {

    SDL_Log("Entering BlockEditorState");
    loadRenderables();
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

}