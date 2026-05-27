#include "../../inc/State/IntroState.h"

typedef ley::Textures TextureManager;

namespace ley {


IntroState::IntroState(ley::Video * v, ley::GameModel * gm)
: BaseState(v, gm) {

   mLogo = ley::Sprite(TextureManager::Instance()->getTexture("sdl"), 0, {}, {1533,{0,0,0,0}});
   mLogo.center();
   mActiveUIElement = {};
}

void IntroState::update(ley::Command command) {

    //note already faded in by default.
    if(mLogo.faderFinished()) {

        switch (mAnimationStep) {
            case 0 :
                mLogo.holdFader();
                mLogo.resetFader();
                break;
            case 1 :
                mLogo.reverseFader();
                mLogo.resetFader();
                break;
            case 2 :
                mLogo = ley::Sprite(TextureManager::Instance()->getTexture("itlogo"), 0, {}, {1533,{0,0,0,0}});
                mLogo.scale(0.75);
                mLogo.center();
                break;
            case 3 :
                mLogo.holdFader();
                mLogo.resetFader();
                break;
            case 4 :
                mLogo.reverseFader();
                mLogo.resetFader();
                break;
            case 5 :
                GameState::done = true;
                break;
            default : ;
        }

        mAnimationStep++;
    }
}

void IntroState::loadRenderables() {
    mRenderables.push_back(&mLogo);
}

bool IntroState::onEnter() {
    SDL_Log("Entering IntroState");
    mLogo.resetFader();
    loadRenderables();
    mCurrentInputContext = "ui";
    return true;
}

bool IntroState::resume() {
    SDL_Log("ReEntering IntroState");
    mCurrentInputContext = "ui";
    return true;
}

} // namespace ley
