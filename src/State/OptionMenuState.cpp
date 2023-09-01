#include "../../inc/State/OptionMenuState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string OptionMenuState::sOptionMenuID = "OPTIONMENU";

OptionMenuState::OptionMenuState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mBackground(ley::Sprite(TextureManager::Instance()->getTexture("optionsmenu"), 0, {})) {

    optionUI.push("options",{0,0,218,63},{29,270,218,63},"btnOptions","options-white","options-hot-red");
    optionUI.push("options1",{0,0,218,63},{29,365,218,63},"btnOptions","options-white","options-hot-red");
    optionUI.push("back",{0,0,100,49},{30,451,100,49},"opt-back","opt-white","opt-hot");
    
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "yes", "yes-white", "yes-hot");
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "no", "no-white", "no-hot");
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "no", "no-hot", "no-white");
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "btnExit","exit-white","exit-hot-red");

    optionUI.addSelector("back", {0,0,100,49}, {300,451,100,49}, "yes", "yes-white", "yes-hot");
    optionUI.addSelector("back", {0,0,100,49}, {300,451,100,49}, "no", "no-white", "no-hot");
}

void OptionMenuState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    optionUI.runCommand(command);
}

void OptionMenuState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer());

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer());
    }

    optionUI.render(mVideoSystem);
}

void OptionMenuState::loadRenderables() {
    mRenderables.push_back(&mBackground);
}

bool OptionMenuState::onEnter() {
    SDL_Log("Entering OptionMenuState");
    loadRenderables();
    return true;
}

bool OptionMenuState::onReEnter() {
    SDL_Log("ReEntering OptionMenuState");
    return true;
}

bool OptionMenuState::onExit() {
    SDL_Log("Exiting OptionMenustate");
    return true;
}

bool OptionMenuState::onPause() {
    GameState::onPause();
    
    return true;
}

}