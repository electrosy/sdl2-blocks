#include "../../inc/State/MenuState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string MenuState::sMenuID = "MENU";

MenuState::MenuState(ley::Video * v, ley::GameModel * gm) {
    mVideoSystem = v;
    mGameModel = gm;
    mBackground = ley::Sprite(TextureManager::Instance()->getTexture("mainmenu"), 0, {}, {1000,{0,0,0,0}});

    // TODO ablockalypse title pulses (gets brighter for a moment) every few moments to grab the players attention if they are just sitting in the main menu.
    mCompanyLogo = ley::Sprite(TextureManager::Instance()->getTexture("itlogo"), 0, {}, {1000,{0,0,0,0}});
    mCompanyLogo.scale(0.25);
    mCompanyLogo.bottomRight();

    mAblockalypseLogo = ley::Sprite(TextureManager::Instance()->getTexture("ablockalypse-logo"), 0, {}, {1000,{0,0,0,0}});
    mAblockalypseLogo.setPos(260,-75);

    //Gather elements for the menus
    mainUI.push("start",{0,0,382,44},{29,199,0,0},"menufonts-base","menufonts-white","menufonts-hot");
    mainUI.push("highscore",{0,45,382,58},{29,282,0,0},"menufonts-base","menufonts-white","menufonts-hot");
    mainUI.push("options",{0,104,382,57},{29,365,0,0},"menufonts-base","menufonts-white","menufonts-hot");
    mainUI.push("exit",{0,163,382,46},{29,451,0,0},"menufonts-base","menufonts-white","menufonts-hot");
    mainUI.push("credits",{0,210,382,46},{29,533,0,0},"menufonts-base","menufonts-white","menufonts-hot");
}

void MenuState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stopProgram(true);
        break;
    }

    if(command == ley::Command::enter && mainUI.getIndex() == 0) {
        mGameModel->stateChange(ley::StateChange::play);
    }

    if(command == ley::Command::enter && mainUI.getIndex() == 1) {
        mGameModel->stateChange(ley::StateChange::highscores);
    }

    if(command == ley::Command::enter && mainUI.getIndex() == 2) {
        mGameModel->stateChange(ley::StateChange::options);
    }

    if(command == ley::Command::enter && mainUI.getIndex() == 3) {
        mGameModel->stateChange(ley::StateChange::quitstate);
        mGameModel->setGameRunning(false);
        mGameModel->stopProgram(true);
    }

    if(command == ley::Command::enter && mainUI.getIndex() == 4) {
        mGameModel->stateChange(ley::StateChange::credits);
    }

    mainUI.runCommand(command);

    mGameModel->audio()->playMainMenu();
}

void MenuState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    mainUI.render(mVideoSystem);
    
    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }
}

void MenuState::loadRenderables() {
    mRenderables.push_back(&mBackground);
    mRenderables.push_back(&mCompanyLogo);
    mRenderables.push_back(&mAblockalypseLogo);
}

bool MenuState::onEnter() {
    SDL_Log("Entering MenuState");    
    loadRenderables();
    return true;
}

bool MenuState::onReEnter() {
    SDL_Log("ReEntering MenuState");
    mBackground.resetFader(); //Reset the fader for this sprite so it will fade in again.
    mAblockalypseLogo.resetFader();
    return true;
}

bool MenuState::onExit() {
    SDL_Log("Exiting Menustate");
    mGameModel->audio()->fadeOutMusic();
    return true;
}

bool MenuState::onPause() {
    GameState::onPause();

    return true;
}

}