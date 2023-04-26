#include "../../inc/State/IntroState.h"

namespace ley {

const std::string IntroState::sIntroID = "INTRO";

IntroState::IntroState(ley::Video * v) {
    videoSystem = v;
}

void IntroState::update(ley::Command command) {

}

void IntroState::loadRenderables() {

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