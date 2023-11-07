#include "../../inc/State/IntroState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string IntroState::sIntroID = "INTRO";

IntroState::IntroState(ley::Video * v, ley::GameModel * gm) {
    mVideoSystem = v;
    mGameModel = gm;

   mLogo = ley::Sprite(TextureManager::Instance()->getTexture("sdl"), 0, {}, {1533,{0,0,0,0}});
   mLogo.center();

}

void IntroState::update(ley::Command command) {
    
    //TODO maybe we want to skip first logo when user presses enter and go to the second logo

    //note already faded in by default.
    if(mLogo.faderFinished()) {

        switch (mAnimationStep) {
            case 0 :
                // hold logo
                mLogo.holdFader();
                mLogo.resetFader();
                break;
            case 1 :
                // fade logo
                mLogo.reverseFader();
                mLogo.resetFader();
                break;
            case 2 :
                // switch to the new logo and fade in
                mLogo = ley::Sprite(TextureManager::Instance()->getTexture("itlogo"), 0, {}, {1533,{0,0,0,0}});
                mLogo.center();
                break;
            case 3 :
                // hold logo
                mLogo.holdFader();
                mLogo.resetFader();
                break;
            case 4 :
                // fade logo out
                mLogo.reverseFader();
                mLogo.resetFader();
                break;
            case 5 :
                // Set the state to finish up.
                GameState::done = true;
                break;
            default : ;
                //nothing
        }

        mAnimationStep++;

    }
}

void IntroState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }
}

void IntroState::loadRenderables() {
    mRenderables.push_back(&mLogo);
}

bool IntroState::onEnter() {
    SDL_Log("Entering IntroState");
    mLogo.resetFader();
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