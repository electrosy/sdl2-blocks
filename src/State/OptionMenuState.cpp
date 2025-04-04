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
    mBoardSizeLabelFont{31,100,10,20},
    mDelayLabelFont{31,150,10,20},
    mRepeatLabelFont{31,200,10,20}

    {

    mLocalTextEntry.setVisible(false);
    mLocalTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mLocalTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mLocalTextEntry.setWidth(85,85,5);
    mLocalTextEntry.setRegEx("\\b(?:[8-9]|1\\d|2[0-5])x(?:[8-9]|1\\d|2[0-2])\\b");
    mLocalTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst) + "," 
        + mGameModel->getLanguageModel()->getWord("e.g. 10x20", 0, false, capitalizationtype::capitalizeNone)
        , "");
    mLocalTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be two numbers seperated by an 'x' between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst));
    mLocalTextEntry.setPos({204,100});

    mDelayTextEntry.setVisible(false);
    mDelayTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mDelayTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mDelayTextEntry.setWidth(85,85,3);
    mLocalTextEntry.setRegEx("\\b(?:[8-9]|1\\d|2[0-5])x(?:[8-9]|1\\d|2[0-2])\\b");


    mOptionUI.pushTextEntry(
        [this](){UI_ToggleFocus();},
        [this]()->bool{return mLocalTextEntry.hasFocus();},
        [this](){commitUI();});


    mOptionUI.pushFont("languageOptions", {29,250,218,63}, mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeFirst), v->getRenderer(), 24);
    mOptionUI.pushFont("keyboardOptions", {29,300,218,63}, mGameModel->getLanguageModel()->getWord("input options", 0, false, capitalizationtype::capitalizeFirst), v->getRenderer(), 24);

    mBoardSizeLabelFont.updateMessage("Board Size");
    mDelayLabelFont.updateMessage("Input Delay");
    mRepeatLabelFont.updateMessage("Input Repeat Rate");
}

void OptionMenuState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    if(command == ley::Command::enter && mOptionUI.getIndex() == 1) {
        mGameModel->stateChange(ley::StateChange::languageoptions);
    }

    if(command == ley::Command::enter && mOptionUI.getIndex() == 2) {
        mGameModel->stateChange(ley::StateChange::keyboardoptions);
    }

    mOptionUI.runCommand(command);

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
        mLocalTextEntry.setTextBoxValue(mPreviousOptionsValue);
    }

    if(mLocalTextEntry.hasFocus()) {
        UI_ToggleFocus();
    }
}

void OptionMenuState::UI_ToggleFocus() {
    
    if(!mLocalTextEntry.hasFocus()){
        mActiveUIElement = &mLocalTextEntry;
        mPreviousOptionsValue = mLocalTextEntry.getTextBoxValue();
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

    mOptionUI.render(mVideoSystem);
}

void OptionMenuState::loadRenderables() {
    mRenderables.push_back(&mBackground);
    mRenderables.push_back(&mLocalTextEntry);
    mRenderables.push_back(&mBoardSizeLabelFont);
    //mRenderables.push_back(&mDelayLabelFont);
    //mRenderables.push_back(&mRepeatLabelFont);
}

bool OptionMenuState::onEnter() {
    SDL_Log("Entering OptionMenuState");

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

    mOptionUI.getElementPtr("languageOptions")->setMessage(mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeFirst));
    mOptionUI.getElementPtr("keyboardOptions")->setMessage(mGameModel->getLanguageModel()->getWord("input options", 0, false, capitalizationtype::capitalizeFirst));

    mLocalTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst) + "," 
        + mGameModel->getLanguageModel()->getWord("e.g. 10x20", 0, false, capitalizationtype::capitalizeNone)
        , "");

    mLocalTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be two numbers seperated by an 'x' between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst));

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