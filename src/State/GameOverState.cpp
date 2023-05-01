#include "../../inc/State/GameOverState.h"

namespace ley {

const std::string GameOverState::sGameOverStateID = "GAMEOVER";

GameOverState::GameOverState(ley::Video * v, ley::GameModel * gm) {

}

void GameOverState::update(ley::Command command) {

}

void GameOverState::loadRenderables() {
    
}

bool GameOverState::onEnter() {
    SDL_Log("Entering GameOverState");
    return true;
}

bool GameOverState::onExit() {
    SDL_Log("Exiting GameOverState");
    return true;
}

}