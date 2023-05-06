#include "../../inc/State/IntroState.h"

namespace ley {

const std::string IntroState::sIntroID = "INTRO";

IntroState::IntroState(ley::Video * v, ley::GameModel * gm) {
    mVideoSystem = v;
    mGameModel = gm;
}

void IntroState::update(ley::Command command) {

}

void IntroState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer());

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer());
    }
}


void IntroState::loadRenderables() {

}

bool IntroState::onEnter() {
    SDL_Log("Entering IntroState");
    return true;
}

bool IntroState::onReEnter() {
    SDL_Log("ReEntering IntroState");
    return true;
}

bool IntroState::onExit() {
    SDL_Log("Exiting Introstate");
    return true;
}

}