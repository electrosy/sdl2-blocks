#include "../../inc/State/GameOverState.h"

namespace ley {

const std::string GameOverState::sGameOverStateID = "GAMEOVER";

GameOverState::GameOverState(ley::Video * v, ley::GameModel * gm)
: 
mVideoSystem(v),
mGameModel(gm),
fontGameOver{255, 190, 100, 35} {

    fontGameOver.updateMessage("Game Over!!!");

}

void GameOverState::update(ley::Command command) {
    switch (command) {
        case ley::Command::enter :
            mGameModel->resetGame();
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
        case ley::Command::quit :
            mGameModel->setGameRunning(false);
//            mGameModel->stopProgram(true);
            mGameModel->stateChange(ley::StateChange::quitstate);
    }
}

void GameOverState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer());

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer());
    }

    fontGameOver.render(mVideoSystem->getRenderer());
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