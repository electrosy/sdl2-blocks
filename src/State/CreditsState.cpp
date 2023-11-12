#include "../../inc/State/CreditsState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string CreditsState::sCreditsID = "CREDITS";

CreditsState::CreditsState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mBackground(ley::Sprite(TextureManager::Instance()->getTexture("credits"), 0, {}, {1000,{0,0,0,0}})),
    mCreditsPanel(ley::Sprite(TextureManager::Instance()->getTexture("credits-panel"), 0, {}, {0,{0,0,0,0}})) {

    mCreditsPanel.center();
}

void CreditsState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    optionUI.runCommand(command);
}

void CreditsState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }

    optionUI.render(mVideoSystem);
}

void CreditsState::loadRenderables() {
    mRenderables.push_back(&mBackground);
    mRenderables.push_back(&mCreditsPanel);
}

bool CreditsState::onEnter() {
    SDL_Log("Entering CreditsState");
    loadRenderables();
    return true;
}

bool CreditsState::onReEnter() {
    SDL_Log("ReEntering CreditsState");
    return true;
}

bool CreditsState::onExit() {
    SDL_Log("Exiting CreditsState");
    return true;
}

bool CreditsState::onPause() {
    GameState::onPause();
    
    return true;
}

}