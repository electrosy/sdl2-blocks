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
    mLocalTextEntry.setPos({224,100});

    mKeyDelayTextEntry.setVisible(false);
    mKeyDelayTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mKeyDelayTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mKeyDelayTextEntry.setWidth(85,85,5);
    mKeyDelayTextEntry.setPos({224,150});
    mKeyDelayTextEntry.setRegEx("^(50|[5-9][0-9]|1[0-9]{2}|2[0-9]{2}|300)$");
    mKeyDelayTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be a number between 50 and 300", 0, false, capitalizationtype::capitalizeFirst));
    mKeyDelayTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 50 and 300", 0, false, capitalizationtype::capitalizeFirst), "");

    mKeyRepeatTextEntry.setVisible(false);
    mKeyRepeatTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mKeyRepeatTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mKeyRepeatTextEntry.setWidth(85,85,5);
    mKeyRepeatTextEntry.setPos({325,200});
    mKeyRepeatTextEntry.setRegEx("^(15|1[6-9]|[2-7][0-9]|80)$");
    mKeyRepeatTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be a number between 15 and 80", 0, false, capitalizationtype::capitalizeFirst));
    mKeyRepeatTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 15 and 80", 0, false, capitalizationtype::capitalizeFirst), "");
    

    mOptionUI.pushTextEntry(
        [this](){mLocalTextEntry.handleFocusChange(&mActiveUIElement, &mPreviousOptionsValue);},
        [this]()->bool{return mLocalTextEntry.hasFocus();},
        [this](){commitBoardSize();});

    //Initialize the active ui element as the first text entry.
    mActiveUIElement = &mLocalTextEntry;
        
    mOptionUI.pushTextEntry(
        [this](){mKeyDelayTextEntry.handleFocusChange(&mActiveUIElement, &mPreviousKeyDelayValue);},
        [this]()->bool{return mKeyDelayTextEntry.hasFocus();},
        [this](){commitKeyDelay();});

    mOptionUI.pushTextEntry(
        [this](){mKeyRepeatTextEntry.handleFocusChange(&mActiveUIElement, &mPreviousKeyRepeatValue);},
        [this]()->bool{return mKeyRepeatTextEntry.hasFocus();},
        [this](){commitKeyRepeat();});

    mOptionUI.pushFont("languageOptions", {29,250,218,63}, mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeFirst), v->getRenderer(), 24);
    mOptionUI.pushFont("keyboardOptions", {29,300,218,63}, mGameModel->getLanguageModel()->getWord("input options", 0, false, capitalizationtype::capitalizeFirst), v->getRenderer(), 24);

    mBoardSizeLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("board size", 0, false, capitalizationtype::capitalizeWords));
    mDelayLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("input delay", 0, false, capitalizationtype::capitalizeWords));
    
    mRepeatLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("input repeat rate", 0, false, capitalizationtype::capitalizeWords));
}

void OptionMenuState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    if(command == ley::Command::enter && mOptionUI.getIndex() == 3) {
        mGameModel->stateChange(ley::StateChange::languageoptions);
    }

    if(command == ley::Command::enter && mOptionUI.getIndex() == 4) {
        mGameModel->stateChange(ley::StateChange::keyboardoptions);
    }

    mOptionUI.runCommand(command);

    mLocalTextEntry.update();
    mKeyDelayTextEntry.update();
    mKeyRepeatTextEntry.update();
}

void OptionMenuState::commitBoardSize() {

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

    mGameModel->readConfigOther();

    if(mLocalTextEntry.hasFocus()) {
        mLocalTextEntry.toggleFocus();
    }
}

void OptionMenuState::commitKeyDelay() {
    if ( std::regex_match(mKeyDelayTextEntry.getTextBoxValue().c_str(), std::regex(mKeyDelayTextEntry.getRegEx()) )) {
        SDL_Log("Regex matched.");

        mGameModel->setKeyDelay(atoi(mKeyDelayTextEntry.getTextBoxValue().c_str()));

    }
    else {
        SDL_Log("Regex did not match: %s ", mKeyDelayTextEntry.getTextBoxValue().c_str());
        mKeyDelayTextEntry.getErrorTimerPtr()->reset();
        mKeyDelayTextEntry.getErrorFontPtr()->setVisible(true);
        
        // TODO can we put more of the text entry logic like previous value into the text entry its self?
        mKeyDelayTextEntry.setTextBoxValue(mPreviousKeyDelayValue);
    }

    if(mKeyDelayTextEntry.hasFocus()) {
        mKeyDelayTextEntry.toggleFocus();
    }
}

void OptionMenuState::commitKeyRepeat() {

    if ( std::regex_match(mKeyRepeatTextEntry.getTextBoxValue().c_str(), std::regex(mKeyRepeatTextEntry.getRegEx()) )) {
        SDL_Log("Regex matched.");

        mGameModel->setKeyRepeat(atoi(mKeyRepeatTextEntry.getTextBoxValue().c_str()));

    }
    else {
        SDL_Log("Regex did not match: %s ", mKeyRepeatTextEntry.getTextBoxValue().c_str());
        mKeyRepeatTextEntry.getErrorTimerPtr()->reset();
        mKeyRepeatTextEntry.getErrorFontPtr()->setVisible(true);
        // TODO can we put more of the text entry logic like previous value into the text entry its self?
        mKeyRepeatTextEntry.setTextBoxValue(mPreviousKeyRepeatValue);
    }

    if(mKeyRepeatTextEntry.hasFocus()) {
        mKeyRepeatTextEntry.toggleFocus();
    }
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
    mRenderables.push_back(&mDelayLabelFont);
    
    mRenderables.push_back(&mKeyDelayTextEntry);
    mRenderables.push_back(&mKeyRepeatTextEntry);
    mRenderables.push_back(&mRepeatLabelFont);
}

bool OptionMenuState::onEnter() {
    SDL_Log("Entering OptionMenuState");

    mLocalTextEntry.setVisible(true);
    mKeyDelayTextEntry.setVisible(true);
    mKeyRepeatTextEntry.setVisible(true);

    loadRenderables();

    
    //load config
    /*
    std::ifstream inFile("config.csv");
    if (inFile.is_open())
    {
        std::string line;
        std::getline(inFile,line);
        std::stringstream ss(line);
        
        SDL_Log("Config read in: %s", line.c_str());

        *mLocalTextEntry.getTextBoxField() = line;
    }
    */

    int boardHeightTotal = mGameModel->getBoard()->height() - BOARDSIZE_BUFFER;
    mLocalTextEntry.setTextBoxValue(std::to_string(mGameModel->getBoard()->width() ) + "x" + std::to_string(boardHeightTotal));
    mKeyDelayTextEntry.setTextBoxValue(std::to_string(mGameModel->getKeyDelay()));
    mKeyRepeatTextEntry.setTextBoxValue(std::to_string(mGameModel->getKeyRepeat()));

    return true;
}

bool OptionMenuState::onReEnter() {
    SDL_Log("ReEntering OptionMenuState");

    initTextEntryMessages();

    return true;
}

bool OptionMenuState::onExit() {
    SDL_Log("Exiting OptionMenustate");

    //Commit the current value in the textentry. This handles cases where the value changes and the player exits the state before moving the cursor up or down.
    commitBoardSize();
    commitKeyDelay();
    commitKeyRepeat();

    return true;
}

bool OptionMenuState::onPause() {
    GameState::onPause();
    
    return true;
}

void OptionMenuState::initTextEntryMessages() {

    mOptionUI.getElementPtr("languageOptions")->setMessage(mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeFirst));
    mOptionUI.getElementPtr("keyboardOptions")->setMessage(mGameModel->getLanguageModel()->getWord("input options", 0, false, capitalizationtype::capitalizeFirst));

    mLocalTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst) + "," 
        + mGameModel->getLanguageModel()->getWord("e.g. 10x20", 0, false, capitalizationtype::capitalizeNone)
        , "");

    mLocalTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be two numbers seperated by an 'x' between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst));

    mKeyRepeatTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be a number between 15 and 80", 0, false, capitalizationtype::capitalizeFirst));
    mKeyRepeatTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 15 and 80", 0, false, capitalizationtype::capitalizeFirst), "");
    
    mRepeatLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("input repeat rate", 0, false, capitalizationtype::capitalizeWords));

    mKeyDelayTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be a number between 50 and 300", 0, false, capitalizationtype::capitalizeFirst));
    mKeyDelayTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 50 and 300", 0, false, capitalizationtype::capitalizeFirst), "");

    mDelayLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("input delay", 0, false, capitalizationtype::capitalizeWords));

    mBoardSizeLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("board size", 0, false, capitalizationtype::capitalizeWords));
}

}