#include "../../inc/State/PlayState.h"

namespace ley {

const std::string PlayState::sPlayID = "PLAY";

void PlayState::update() {

}

void PlayState::render() {

}

bool PlayState::onEnter() {
    SDL_Log("Entering PlayState");
    return true;
}

bool PlayState::onExit() {
    SDL_Log("Exiting Playstate");
    return true;
}

}