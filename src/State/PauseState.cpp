#include "../../inc/State/PauseState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string PauseState::sPauseID = "PAUSE";

PauseState::PauseState(ley::Video * v, ley::GameModel * gm)
:
mVideoSystem(v),
mGameModel(gm),
controlsSprite{TextureManager::Instance()->getTexture("game-controls"), 0, {}, {0,{0,0,0,0}}} {

   controlsSprite.center();
}

void PauseState::update(ley::Command command) {

    switch (command) {
        case ley::Command::pause :
            mGameModel->pauseGame(!mGameModel->isPaused());
        break;
        case ley::Command::debugcolide :
            //new board
            mGameModel->getBoard()->debugOutput(true);
        break;
        case ley::Command::debugtexture :
            //new board
            mGameModel->getBoard()->debugOutput(true);
            
        break;
        case ley::Command::quit :
        break;
        case ley::Command::nextSong :
        break;
    }

    /**** UPDATE ****/
    mGameModel->audio()->playPlaylist();
}

void PauseState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }
}

void PauseState::loadRenderables() {
    
    mRenderables.push_back(&controlsSprite);
}

bool PauseState::onEnter() {
    SDL_Log("Entering PauseState and loading renderables");
    loadRenderables();
    return true;
}

bool PauseState::onReEnter() {
    SDL_Log("ReEntering PauseState");
    return true;
}

bool PauseState::onExit() {    
    SDL_Log("Exiting Pausestate");
    return true;
}

bool PauseState::onPause() {
    GameState::onPause();
    return true;
}

}