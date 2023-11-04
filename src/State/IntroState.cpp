#include "../../inc/State/IntroState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string IntroState::sIntroID = "INTRO";

IntroState::IntroState(ley::Video * v, ley::GameModel * gm) {
    mVideoSystem = v;
    mGameModel = gm;

    mBackground = ley::Sprite(TextureManager::Instance()->getTexture("sdl"), 0, {}, {1000,{0,0,0,0}});
    mBackground.center();
    
}

void IntroState::update(ley::Command command) {

}

void IntroState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), true);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }
}

void IntroState::loadRenderables() {
    mRenderables.push_back(&mBackground);
}

bool IntroState::onEnter() {
    SDL_Log("Entering IntroState");
    mBackground.resetFader();
    loadRenderables();
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

bool IntroState::onPause() {
    GameState::onPause();
    
    return true;
}

}