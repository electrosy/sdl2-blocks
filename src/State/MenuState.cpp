#include "../../inc/State/MenuState.h"

namespace ley {

const std::string MenuState::sMenuID = "MENU";

MenuState::MenuState(ley::Video * v, ley::GameModel * gm) {
    mVideoSystem = v;
    mGameModel = gm;
}

void MenuState::update(ley::Command command) {

}

void MenuState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer());

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer());
    }
}

void MenuState::loadRenderables() {

}

bool MenuState::onEnter() {
    SDL_Log("Entering MenuState");
    return true;
}

bool MenuState::onReEnter() {
    SDL_Log("ReEntering MenuState");
    return true;
}

bool MenuState::onExit() {
    SDL_Log("Exiting Menustate");
    return true;
}

}