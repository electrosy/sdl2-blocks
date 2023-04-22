#include "../../inc/State/MenuState.h"

namespace ley {

const std::string MenuState::sMenuID = "MENU";

MenuState::MenuState(ley::Video * v) {
    videoSystem = v;
}

void MenuState::update(ley::Command command, ley::GameModel * gm) {

}

void MenuState::loadRenderables() {

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