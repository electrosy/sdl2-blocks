#include "../../inc/State/MenuState.h"

namespace ley {

const std::string MenuState::sMenuID = "MENU";

void MenuState::update() {

}

void MenuState::render() {

}

bool MenuState::onEnter() {
    SDL_Log("Entering MenuState");
    return true;
}

bool MenuState::onExit() {
    SDL_Log("Exiting Menustate");
    return true;
}

}