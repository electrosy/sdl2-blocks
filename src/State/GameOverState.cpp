#include "../../inc/State/GameOverState.h"

namespace ley {


GameOverState::GameOverState(ley::Video * v, ley::GameModel * gm)
: 
BaseState(v, gm),
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
    BaseState::render();
    fontGameOver.render(mVideoSystem->getRenderer(), false);
}

void GameOverState::loadRenderables() {
    
}

bool GameOverState::onEnter() {
    SDL_Log("Entering GameOverState");
    mCurrentInputContext = "play";
    return true;
}

bool GameOverState::resume() {
    SDL_Log("ReEntering GameOverState");
    mCurrentInputContext = "play";
    return true;
}


}