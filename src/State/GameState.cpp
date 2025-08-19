#include "../../inc/State/GameState.h"

namespace ley {

UIWidget* GameState::mActiveUIElement = {};
std::string GameState::mCurrentInputContext = "";

bool GameState::onPause() {
    std::string pause = "Pausing " + getStateID();
    SDL_Log(pause.c_str());

    return true;
}

bool GameState::isDone() {
    return done;
}

void GameState::commitUI() {
    ;
}

ley::UIWidget* GameState::activeUIelement() {
    return mActiveUIElement;
}

std::string GameState::currentInputContext() {
    return mCurrentInputContext;
}

void GameState::UI_ToggleFocus() {
    
}

}