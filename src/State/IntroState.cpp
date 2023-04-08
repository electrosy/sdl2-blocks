#include "../../inc/State/IntroState.h"

namespace ley {

const std::string IntroState::sIntroID = "INTRO";

void IntroState::update() {

}

void IntroState::render() {

}

bool IntroState::onEnter() {
    SDL_Log("Entering IntroState");
    return true;
}

bool IntroState::onExit() {
    SDL_Log("Exiting Introstate");
    return true;
}

}