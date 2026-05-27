#include "../../inc/State/PauseState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string PauseState::sPauseID = "PAUSE";

PauseState::PauseState(ley::Video * v, ley::GameModel * gm)
:
BaseState(v, gm),
controlsSprite{TextureManager::Instance()->getTexture("game-controls"), 0, {}, {0,{0,0,0,0}}} {

   controlsSprite.center();
   mActiveUIElement = {};
}

void PauseState::update(ley::Command command) {

    switch (command) {
        case ley::Command::pause :
            mGameModel->pauseGame(!mGameModel->isPaused());
        break;
        case ley::Command::debugcolide :
            mGameModel->getBoard()->debugOutput(true);
        break;
        case ley::Command::debugtexture :
            mGameModel->getBoard()->debugOutput(true);
        break;
        case ley::Command::quit :
        break;
        case ley::Command::nextSong :
        break;
    }

    mGameModel->audio()->playPlaylist();
}

void PauseState::loadRenderables() {
    mRenderables.push_back(&controlsSprite);
}

bool PauseState::onEnter() {
    SDL_Log("Entering PauseState and loading renderables");
    loadRenderables();
    mCurrentInputContext = "play";
    return true;
}

bool PauseState::resume() {
    SDL_Log("ReEntering PauseState");
    mCurrentInputContext = "play";
    return true;
}

} // namespace ley
