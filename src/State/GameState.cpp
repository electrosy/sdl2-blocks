#include "../../inc/State/GameState.h"

namespace ley {

bool GameState::onPause() {
    std::string pause = "Pausing " + getStateID();
    SDL_Log(pause.c_str());

    return true;
}

bool GameState::isDone() {
    return done;
}

void GameState::onCommandEnter() {
    ;
}

// TODO this mTextEntry can no go into the individual states and this method can be removed.
ley::TextEntry* GameState::textEntry() {
    return &mTextEntry;
}

void GameState::UI_ToggleFocus() {
    ;
}

}