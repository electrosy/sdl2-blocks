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
    mBackground(ley::Sprite(TextureManager::Instance()->getTexture("optionsmenu"), 0, {}, {1000,{0,0,0,0}}))

    {

    mLocalTextEntry.setVisible(false);
    mLocalTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mLocalTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mLocalTextEntry.setWidth(200,200,30);
    mLocalTextEntry.setRegEx("\\b(?:[8-9]|1\\d|2[0-5])x(?:[8-9]|1\\d|2[0-2])\\b");
    mLocalTextEntry.setHelpMessages("Enter a number between 8x8 - 25x22, e.g. \"10x20\" and press down.", "Scroll here to modify field.");

    optionUI.pushTextEntry(
        [this](){UI_ToggleFocus();},
        [this]()->bool{return mLocalTextEntry.hasFocus();},
        [this](){commitUI();});

    optionUI.pushFont("keyboardOptions", {29,200,218,63}, "Input Options", v->getRenderer());
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

    if(command == ley::Command::enter && optionUI.getIndex() == 1) {
        mGameModel->stateChange(ley::StateChange::keyboardoptions);
    }

    optionUI.runCommand(command);

    mLocalTextEntry.update();
}

void OptionMenuState::commitUI() {

    SDL_Log("OptionMenuState::commitUI()");
    //TODO maybe this regex check should be contained within the TextEntry
    if ( std::regex_match(mLocalTextEntry.getTextBoxValue().c_str(), std::regex(mLocalTextEntry.getRegEx()) )) {
        SDL_Log("Regex matched.");

        //save the config only if we have a new value that is valid.
        std::ofstream myfile;
        myfile.open ("config.csv");
        myfile << mLocalTextEntry.getTextBoxValue() << std::endl;
        myfile.close();
    }
    else {
        SDL_Log("Regex did not match: %s ", mLocalTextEntry.getTextBoxValue().c_str());
        mLocalTextEntry.getErrorTimerPtr()->reset();
        mLocalTextEntry.getErrorFontPtr()->setVisible(true);
        // TODO can we put more of the text entry logic like previous value into the text entry its self?
        mLocalTextEntry.setTextBoxValue(previousOptionsValue);
    }

    if(mLocalTextEntry.hasFocus()) {
        UI_ToggleFocus();
    }
}

void OptionMenuState::UI_ToggleFocus() {
    
    if(!mLocalTextEntry.hasFocus()){
        mActiveUIElement = &mLocalTextEntry;
        previousOptionsValue = mLocalTextEntry.getTextBoxValue();
    }
    else {
        mActiveUIElement = {};
    }

    mLocalTextEntry.toggleFocus();
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
    mRenderables.push_back(&mLocalTextEntry);
}

bool OptionMenuState::onEnter() {
    SDL_Log("Entering OptionMenuState");

    mLocalTextEntry.setPos({31,100});
    mLocalTextEntry.setVisible(true);

    loadRenderables();

    //load config
    std::ifstream inFile("config.csv");
    if (inFile.is_open())
    {
        std::string line;
        std::getline(inFile,line);
        std::stringstream ss(line);
        
        SDL_Log("Config read in: %s", line.c_str());

        *mLocalTextEntry.getTextBoxField() = line;
    }

    return true;
}

bool OptionMenuState::onReEnter() {
    SDL_Log("ReEntering OptionMenuState");
    return true;
}

bool OptionMenuState::onExit() {
    SDL_Log("Exiting OptionMenustate");

    //Commit the current value into the textentry
    commitUI();

    return true;
}

bool OptionMenuState::onPause() {
    GameState::onPause();
    
    return true;
}

}