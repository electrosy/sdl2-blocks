#include "../../inc/State/GameOverState.h"

namespace ley {

const std::string GameOverState::sGameOverStateID = "GAMEOVER";

GameOverState::GameOverState(ley::Video * v, ley::GameModel * gm)
: 
mVideoSystem(v),
mGameModel(gm),
fontGameOver{255, 190, 100, 35} {

    fontGameOver.updateMessage(gm->getLanguageModel()->getWord("game over", 0, false, capitalizationtype::capitalizeWords) + "!!!");

    mActiveUIElement = {};
}

void GameOverState::update(ley::Command command) {
    switch (command) {
        case ley::Command::enter :
            mGameModel->resetGame();
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
        case ley::Command::quit :
            mGameModel->setGameRunning(false);
            mGameModel->stateChange(ley::StateChange::quitstate);
    }
}

void GameOverState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }

    fontGameOver.render(mVideoSystem->getRenderer(), false);
}

void GameOverState::loadRenderables() {
    
}

bool GameOverState::onEnter() {
    SDL_Log("Entering GameOverState");
    mCurrentInputContext = "play";
    return true;
}

bool GameOverState::onReEnter() {
    SDL_Log("ReEntering GameOverState");
    mCurrentInputContext = "play";
    return true;
}

bool GameOverState::onExit() {
    SDL_Log("Exiting GameOverState");
    return true;
}

bool GameOverState::onPause() {
    GameState::onPause();
    
    return true;
}

}