#include "../../inc/State/OptionMenuState.h"

#include <fstream>
#include <sstream>
#include <regex>

typedef ley::Textures TextureManager;

namespace ley {

const std::string OptionMenuState::sOptionMenuID = "OPTIONMENU";

OptionMenuState::OptionMenuState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mBackground(ley::Sprite(TextureManager::Instance()->getTexture("optionsmenu"), 0, {}, {1000,{0,0,0,0}})),
    mTextErrorTimer(2500, {0,0,0,0}) {

    optionUI.push("options",{0,0,218,63},{29,270,218,63},"btnOptions","options-white","options-hot-red");
    optionUI.push("options1",{0,0,218,63},{29,365,218,63},"btnOptions","options-white","options-hot-red");
    optionUI.push("back",{0,0,100,49},{30,451,100,49},"opt-back","opt-white","opt-hot");
    
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "yes", "yes-white", "yes-hot");
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "no", "no-white", "no-hot");
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "no", "no-hot", "no-white");
    optionUI.addSelector("options1", {0,0,100,49}, {300,365,100,49}, "btnExit","exit-white","exit-hot-red");

    optionUI.addSelector("back", {0,0,100,49}, {300,451,100,49}, "yes", "yes-white", "yes-hot");
    optionUI.addSelector("back", {0,0,100,49}, {300,451,100,49}, "no", "no-white", "no-hot");

    mTextEntry.setVisible(false);
    mTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});

    mTextErrorMessage.updateMessage("");
}

void OptionMenuState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
        case ley::Command::tab :
            mGameModel->UIInputFocus(ley::UIFocusChange::goto_textBox);
            previousOptionsValue = mTextEntry.getTextBoxValue();
    }

    optionUI.runCommand(command);
    mTextErrorTimer.runFrame(false, 0.0);
    if(mTextErrorTimer.hasExpired()) {
        mTextErrorMessage.updateMessage("");
    }
}

void OptionMenuState::onCommandEnter() {

    SDL_Log("OptionMenuState::onCommandEnter()");

    std::ofstream myfile;
    myfile.open ("config.csv");

    if ( std::regex_match(mTextEntry.getTextBoxValue().c_str(), std::regex("\\b(?:[8-9]|1\\d|2[0-5])x(?:[8-9]|1\\d|2[0-2])\\b") )) {
        SDL_Log("Regex matched.");
    }
    else {
        SDL_Log("Regex did not match");
        mTextErrorMessage.updateMessage("Must be two numbers seperated by an 'x' between 8x8 and 25x22");
        mTextErrorTimer.reset();
        // TODO can we put more of the text entry logic like previous value into the text entry its self?
        mTextEntry.setTextBoxValue(previousOptionsValue);
    }

    myfile << mTextEntry.getTextBoxValue() << std::endl;
    myfile.close();
}

void OptionMenuState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }

    optionUI.render(mVideoSystem);
}

void OptionMenuState::loadRenderables() {
    mRenderables.push_back(&mBackground);
    mRenderables.push_back(&mTextEntry);
    mRenderables.push_back(&mTextErrorMessage);
}

bool OptionMenuState::onEnter() {
    SDL_Log("Entering OptionMenuState");

    mTextEntry.setPos({10,10});
    mTextEntry.setVisible(true);

    loadRenderables();


    //load config
    std::ifstream inFile("config.csv");
    if (inFile.is_open())
    {
        std::string line;
        std::getline(inFile,line);
        std::stringstream ss(line);
        
        SDL_Log("Config read in: %s", line.c_str());

        *mTextEntry.getTextBoxField() = line;
    }

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