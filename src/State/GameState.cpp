#include "../../inc/State/GameState.h"

namespace ley {

TextEntry* GameState::mActiveUIElement = {};

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

ley::TextEntry* GameState::activeUIelement() {
    return mActiveUIElement;
}

void GameState::UI_ToggleFocus() {
    
}

}