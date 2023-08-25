#include "../../inc/State/MenuState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string MenuState::sMenuID = "MENU";

MenuState::MenuState(ley::Video * v, ley::GameModel * gm) {
    mVideoSystem = v;
    mGameModel = gm;
    mBackground = ley::Sprite(TextureManager::Instance()->getTexture("mainmenu"), 0, {});

    //Gather elements for the menus
    mainUI.push("start",{0,0,139,46},{25,199,139,46},"btnStart","start-white","start-hot-red");
    mainUI.push("highscore",{0,0,323,64},{29,282,323,64},"btnHighScores","highscores-white","highscores-hot-red");
    mainUI.push("options",{0,0,218,63},{29,365,218,63},"btnOptions","options-white","options-hot-red");
    mainUI.push("exit",{0,0,100,49},{30,451,100,49},"btnExit","exit-white","exit-hot-red");

}

void MenuState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->setGameRunning(false);
            mGameModel->stopProgram(true);
        break;
    }

    if(command == ley::Command::enter && mainUI.getIndex() == 0) {
        mGameModel->stateChange(ley::StateChange::play);
    }

    if(command == ley::Command::enter && mainUI.getIndex() == 2) {
        mGameModel->stateChange(ley::StateChange::options);
    }

    mainUI.runCommand(command);
}

void MenuState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer());

    mainUI.render(mVideoSystem);
    
    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer());
    }
}

void MenuState::loadRenderables() {
    mRenderables.push_back(&mBackground);
}

bool MenuState::onEnter() {
    SDL_Log("Entering MenuState");
    loadRenderables();
    return true;
}

bool MenuState::onReEnter() {
    SDL_Log("ReEntering MenuState");
    return true;
}

bool MenuState::onExit() {
    SDL_Log("Exiting Menustate");
    return true;
}

}