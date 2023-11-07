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

}