#include "../../inc/State/GameOverState.h"

namespace ley {

const std::string GameOverState::sGameOverStateID = "GAMEOVER";

GameOverState::GameOverState(ley::Video * v, ley::GameModel * gm)
: 
mVideoSystem(v),
mGameModel(gm) {

}

void GameOverState::update(ley::Command command) {
    switch (command) {
        case ley::Command::enter :
            mGameModel->resetGame();
        break;
    }
}

void GameOverState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer());

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer());
    }
}

void GameOverState::loadRenderables() {
    
}

bool GameOverState::onEnter() {
    SDL_Log("Entering GameOverState");
    return true;
}

bool GameOverState::onReEnter() {
    SDL_Log("ReEntering GameOverState");
    return true;
}

bool GameOverState::onExit() {
    SDL_Log("Exiting GameOverState");
    return true;
}

}