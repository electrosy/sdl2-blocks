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
    mRepeatLabelFont{31,200,10,20},
    mGuideGridOnLabelFont{31,250,10,20},
    mWallKickOnLabelFont{31,300,10,20},
    mDropCoolDownLabelFont{31,350,10,20}

    {
    // TODO streamline the text entry field an make the logic a little more generic so that its more straight forward to add new options.
    // TODO Clean up setHelpMessage and setErrorMessage methods as they should all be in the init function.
    mBoardSizeTextEntry.setVisible(false);
    mBoardSizeTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mBoardSizeTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mBoardSizeTextEntry.setWidthByChar(5);
    mBoardSizeTextEntry.setRegEx("\\b(?:[8-9]|1\\d|2[0-5])x(?:[8-9]|1\\d|2[0-2])\\b");
    mBoardSizeTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst) + "," 
        + mGameModel->getLanguageModel()->getWord("e.g. 10x20", 0, false, capitalizationtype::capitalizeNone)
        , "");
    mBoardSizeTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be two numbers seperated by an 'x' between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst));
    mBoardSizeTextEntry.setPos({224,100});

    mKeyDelayTextEntry.setVisible(false);
    mKeyDelayTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mKeyDelayTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mKeyDelayTextEntry.setWidthByChar(3);
    mKeyDelayTextEntry.setPos({224,150});
    mKeyDelayTextEntry.setRegEx("^(50|[5-9][0-9]|1[0-9]{2}|2[0-9]{2}|300)$");
    mKeyDelayTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be a number between 50 and 300", 0, false, capitalizationtype::capitalizeFirst));
    mKeyDelayTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 50 and 300", 0, false, capitalizationtype::capitalizeFirst), "");

    mKeyRepeatTextEntry.setVisible(false);
    mKeyRepeatTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mKeyRepeatTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mKeyRepeatTextEntry.setWidthByChar(2);
    mKeyRepeatTextEntry.setPos({325,200});
    mKeyRepeatTextEntry.setRegEx("^(15|1[6-9]|[2-7][0-9]|80)$");
    mKeyRepeatTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be a number between 15 and 80", 0, false, capitalizationtype::capitalizeFirst));
    mKeyRepeatTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 15 and 80", 0, false, capitalizationtype::capitalizeFirst), "");

    mGuideGridOnTextEntry.setVisible(false);
    mGuideGridOnTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mGuideGridOnTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mGuideGridOnTextEntry.setWidthByChar(6);
    mGuideGridOnTextEntry.setPos({325,250});
    mGuideGridOnTextEntry.setRegEx("^(off|red|green|yellow|cyan|purple)$");
    mGuideGridOnTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be one of: off, red, green, yellow, cyan, purple", 0, false, capitalizationtype::capitalizeFirst));
    mGuideGridOnTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter one of: off, red, green, yellow, cyan, purple", 0, false, capitalizationtype::capitalizeFirst), "");

    mWallKickOnTextEntry.setVisible(false);
    mWallKickOnTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mWallKickOnTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mWallKickOnTextEntry.setWidthByChar(3);
    mWallKickOnTextEntry.setPos({325,300});
    mWallKickOnTextEntry.setRegEx("^(off|on)$");
    mWallKickOnTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be one of: off, on", 0, false, capitalizationtype::capitalizeFirst));
    mWallKickOnTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter one of: off, on", 0, false, capitalizationtype::capitalizeFirst), "");

    mDropCoolDownTextEntry.setVisible(false);
    mDropCoolDownTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mDropCoolDownTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});
    mDropCoolDownTextEntry.setWidthByChar(3);
    mDropCoolDownTextEntry.setPos({325,350});
    mDropCoolDownTextEntry.setRegEx("^(?:[0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|250)$");
    mDropCoolDownTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be a number between 0 and 250", 0, false, capitalizationtype::capitalizeFirst));
    mDropCoolDownTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 0 and 250", 0, false, capitalizationtype::capitalizeFirst), "");

    mOptionUI.pushUIElement(
        [this](){mBoardSizeTextEntry.handleFocusChange(&mActiveUIElement, &mPreviousOptionsValue);},
        [this]()->bool{return mBoardSizeTextEntry.hasFocus();},
        [this](){commitBoardSize();});

    //Initialize the active ui element as the first text entry.
    mActiveUIElement = &mBoardSizeTextEntry;
        
    mOptionUI.pushUIElement(
        [this](){mKeyDelayTextEntry.handleFocusChange(&mActiveUIElement, &mPreviousKeyDelayValue);},
        [this]()->bool{return mKeyDelayTextEntry.hasFocus();},
        [this](){ commitKeyDelay(); });

    mOptionUI.pushUIElement(
        [this](){mKeyRepeatTextEntry.handleFocusChange(&mActiveUIElement, &mPreviousKeyRepeatValue);},
        [this]()->bool{return mKeyRepeatTextEntry.hasFocus();},
        [this](){ commitKeyRepeat(); });

    mOptionUI.pushUIElement(
        [this](){mGuideGridOnTextEntry.handleFocusChange(&mActiveUIElement, &mPreviousGuideGridOnValue);},
        [this]()->bool{return mGuideGridOnTextEntry.hasFocus();},
        [this](){ commitGuideGridOn(); });

    mOptionUI.pushUIElement(
        [this](){mWallKickOnTextEntry.handleFocusChange(&mActiveUIElement, &mPreviousWallKickOnValue);},
        [this]()->bool{return mWallKickOnTextEntry.hasFocus();},
        [this](){ commitWallKickOn(); });

    mOptionUI.pushUIElement(
        [this](){mDropCoolDownTextEntry.handleFocusChange(&mActiveUIElement, &mPreviousDropCoolDownValue);},
        [this]()->bool{return mDropCoolDownTextEntry.hasFocus();},
        [this](){ commitHardDropCoolDown(); });

    mOptionUI.pushFont("languageOptions", {29,400,218,63}, mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeFirst), v->getRenderer(), 24);
    mOptionUI.pushFont("keyboardOptions", {29,450,218,63}, mGameModel->getLanguageModel()->getWord("input options", 0, false, capitalizationtype::capitalizeFirst), v->getRenderer(), 24);
    // TODO localization
    mOptionUI.pushFont("blockEditor", {29,500,218,63}, mGameModel->getLanguageModel()->getWord("block editor", 0, false, capitalizationtype::capitalizeFirst), v->getRenderer(), 24);


    mBoardSizeLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("board size", 0, false, capitalizationtype::capitalizeWords));
    mDelayLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("input delay", 0, false, capitalizationtype::capitalizeWords));
    
    // TODO localization
    mGuideGridOnLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("guide grid on", 0, false, capitalizationtype::capitalizeWords));
    
    mRepeatLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("input repeat rate", 0, false, capitalizationtype::capitalizeWords));

    // TODO localization
    mWallKickOnLabelFont.updateMessage("Wall Kick");
    mDropCoolDownLabelFont.updateMessage("Quick Drop Cool down");
}

void OptionMenuState::update(ley::Command command) {

    switch (command) {
        case ley::Command::UI_back :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    if(command == ley::Command::UI_enter && mOptionUI.getIndex() == 6) {
        mGameModel->stateChange(ley::StateChange::languageoptions);
    }

    if(command == ley::Command::UI_enter && mOptionUI.getIndex() == 7) {
        mGameModel->stateChange(ley::StateChange::keyboardoptions);
    }

    if(command == ley::Command::UI_enter && mOptionUI.getIndex() == 8) {
        mGameModel->stateChange(ley::StateChange::blockeditor);
    }

    mOptionUI.runCommand(command);

    mBoardSizeTextEntry.update();
    mKeyDelayTextEntry.update();
    mKeyRepeatTextEntry.update();
    mGuideGridOnTextEntry.update();
    mWallKickOnTextEntry.update();
}

void OptionMenuState::commitBoardSize() {

    SDL_Log("OptionMenuState::commitUI()");
    if ( std::regex_match(mBoardSizeTextEntry.getTextBoxValue().c_str(), std::regex(mBoardSizeTextEntry.getRegEx()) )) {
        SDL_Log("Regex matched.");

        //save the config only if we have a new value that is valid.
        std::ofstream myfile;
        myfile.open ("config.csv");
        myfile << mBoardSizeTextEntry.getTextBoxValue() << std::endl;
        myfile.close();
    }
    else {
        SDL_Log("Regex did not match: %s ", mBoardSizeTextEntry.getTextBoxValue().c_str());
        mBoardSizeTextEntry.getErrorTimerPtr()->reset();
        mBoardSizeTextEntry.getErrorFontPtr()->setVisible(true);
        mBoardSizeTextEntry.setTextBoxValue(mPreviousOptionsValue);
    }

    mGameModel->readConfigOther();

    if(mBoardSizeTextEntry.hasFocus()) {
        mBoardSizeTextEntry.toggleFocus();
    }
}

void OptionMenuState::commitKeyDelay() {

    if(mKeyDelayTextEntry.commit(mPreviousKeyDelayValue)) {
         mGameModel->setKeyDelay(atoi(mKeyDelayTextEntry.getTextBoxValue().c_str()));
    }
}

void OptionMenuState::commitKeyRepeat() {

    if(mKeyRepeatTextEntry.commit(mPreviousKeyRepeatValue)) {
         mGameModel->setKeyRepeat(atoi(mKeyRepeatTextEntry.getTextBoxValue().c_str()));
    }
}

void OptionMenuState::commitGuideGridOn() {

    if(mGuideGridOnTextEntry.commit(mPreviousGuideGridOnValue)) {
         mGameModel->setGuideGridOn(mGuideGridOnTextEntry.getTextBoxValue()); 
    }
}

void OptionMenuState::commitWallKickOn() {

    if(mWallKickOnTextEntry.commit(mPreviousWallKickOnValue)) {
        mGameModel->setWallKickOn(mWallKickOnTextEntry.getTextBoxValue() == "on" ? "on" : "off" );
    }
}

void OptionMenuState::commitHardDropCoolDown() {

    if(mDropCoolDownTextEntry.commit(mPreviousDropCoolDownValue)) {
        mGameModel->setDropCoolDown(std::stoi(mDropCoolDownTextEntry.getTextBoxValue()));
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
    
    //background
    mRenderables.push_back(&mBackground);
    
    //labels
    mRenderables.push_back(&mBoardSizeLabelFont);
    mRenderables.push_back(&mDelayLabelFont);
    mRenderables.push_back(&mRepeatLabelFont);
    mRenderables.push_back(&mGuideGridOnLabelFont);
    mRenderables.push_back(&mWallKickOnLabelFont);
    mRenderables.push_back(&mDropCoolDownLabelFont);
    
    //text entries
    mRenderables.push_back(&mBoardSizeTextEntry);
    mRenderables.push_back(&mKeyDelayTextEntry);
    mRenderables.push_back(&mKeyRepeatTextEntry);
    mRenderables.push_back(&mGuideGridOnTextEntry);
    mRenderables.push_back(&mWallKickOnTextEntry);
    mRenderables.push_back(&mDropCoolDownTextEntry);
}

bool OptionMenuState::onEnter() {
    SDL_Log("Entering OptionMenuState");

    mBoardSizeTextEntry.setVisible(true);
    mKeyDelayTextEntry.setVisible(true);
    mKeyRepeatTextEntry.setVisible(true);
    mGuideGridOnTextEntry.setVisible(true);
    mWallKickOnTextEntry.setVisible(true);
    mDropCoolDownTextEntry.setVisible(true);

    loadRenderables();

    int boardHeightTotal = mGameModel->getBoard()->height() - BOARDSIZE_BUFFER;
    mBoardSizeTextEntry.setTextBoxValue(std::to_string(mGameModel->getBoard()->width() ) + "x" + std::to_string(boardHeightTotal));
    mKeyDelayTextEntry.setTextBoxValue(std::to_string(mGameModel->getKeyDelay()));
    mKeyRepeatTextEntry.setTextBoxValue(std::to_string(mGameModel->getKeyRepeat()));
    mGuideGridOnTextEntry.setTextBoxValue(mGameModel->getGuideGridOn());
    mWallKickOnTextEntry.setTextBoxValue(  mGameModel->getWallKickOn() == "on" ? "on" : "off" );
    mDropCoolDownTextEntry.setTextBoxValue( std::to_string(mGameModel->getDropCoolDown()));


    positionOptionsLabels();
    mCurrentInputContext = "ui";

    return true;
}

bool OptionMenuState::onReEnter() {
    SDL_Log("ReEntering OptionMenuState");

    initTextEntryMessages();
    positionOptionsLabels();
    mCurrentInputContext = "ui";

    return true;
}

bool OptionMenuState::onExit() {
    SDL_Log("Exiting OptionMenustate");

    //Commit the current value in the textentry. This handles cases where the value changes and the player exits the state before moving the cursor up or down.
    commitBoardSize();
    commitKeyDelay();
    commitKeyRepeat();
    commitGuideGridOn();
    commitWallKickOn();
    commitHardDropCoolDown();
    
    mActiveUIElement = {};

    return true;
}

bool OptionMenuState::onPause() {
    GameState::onPause();
    
    return true;
}

void OptionMenuState::initTextEntryMessages() {

    mOptionUI.getElementPtr("languageOptions")->setMessage(mGameModel->getLanguageModel()->getWord("language options", 0, false, capitalizationtype::capitalizeFirst));
    mOptionUI.getElementPtr("keyboardOptions")->setMessage(mGameModel->getLanguageModel()->getWord("input options", 0, false, capitalizationtype::capitalizeFirst));

    mBoardSizeTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst) + "," 
        + mGameModel->getLanguageModel()->getWord("e.g. 10x20", 0, false, capitalizationtype::capitalizeNone)
        , "");

    mBoardSizeTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be two numbers seperated by an 'x' between 8x8 and 25x22", 0, false, capitalizationtype::capitalizeFirst));

    mKeyRepeatTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be a number between 15 and 80", 0, false, capitalizationtype::capitalizeFirst));
    mKeyRepeatTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 15 and 80", 0, false, capitalizationtype::capitalizeFirst), "");
    
    mRepeatLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("input repeat rate", 0, false, capitalizationtype::capitalizeWords));

    mKeyDelayTextEntry.setErrorMessage(mGameModel->getLanguageModel()->getWord("must be a number between 50 and 300", 0, false, capitalizationtype::capitalizeFirst));
    mKeyDelayTextEntry.setHelpMessages(mGameModel->getLanguageModel()->getWord("enter a number between 50 and 300", 0, false, capitalizationtype::capitalizeFirst), "");

    mDelayLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("input delay", 0, false, capitalizationtype::capitalizeWords));
    mBoardSizeLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("board size", 0, false, capitalizationtype::capitalizeWords));
    mGuideGridOnLabelFont.updateMessage(mGameModel->getLanguageModel()->getWord("guide grid on", 0, false, capitalizationtype::capitalizeWords));
    mWallKickOnLabelFont.updateMessage("Wall Kick");
    mDropCoolDownLabelFont.updateMessage("Quick Drop Cool down");
}

void OptionMenuState::positionOptionsLabels() {
    
    int w = 0; 
    int h = 0;
    SDL_Point labelPos{0,0};
    
    // get the spacing of a " " character space.
    TTF_SizeUTF8(mBoardSizeLabelFont.getTTFFont(), " ", &w, &h );
    int labelDataSpacing = w;

    // TODO we need to generalize this code so that it can be more automatic. Maybe put in a container of objects needing
    TTF_SizeUTF8( mBoardSizeLabelFont.getTTFFont(), mBoardSizeLabelFont.getMessage().c_str(), &w, &h );
    labelPos = mBoardSizeLabelFont.getPos();
    mBoardSizeTextEntry.setPos({labelPos.x + w + labelDataSpacing, labelPos.y});

    TTF_SizeUTF8( mDelayLabelFont.getTTFFont(), mDelayLabelFont.getMessage().c_str(), &w, &h );
    labelPos = mDelayLabelFont.getPos();
    mKeyDelayTextEntry.setPos({labelPos.x + w + labelDataSpacing, labelPos.y});

    TTF_SizeUTF8( mRepeatLabelFont.getTTFFont(), mRepeatLabelFont.getMessage().c_str(), &w, &h );
    labelPos = mRepeatLabelFont.getPos();
    mKeyRepeatTextEntry.setPos({labelPos.x + w + labelDataSpacing, labelPos.y});

    TTF_SizeUTF8( mGuideGridOnLabelFont.getTTFFont(), mGuideGridOnLabelFont.getMessage().c_str(), &w, &h );
    labelPos = mGuideGridOnLabelFont.getPos();
    mGuideGridOnTextEntry.setPos({labelPos.x + w + labelDataSpacing, labelPos.y});

    TTF_SizeUTF8( mWallKickOnLabelFont.getTTFFont(), mWallKickOnLabelFont.getMessage().c_str(), &w, &h );
    labelPos = mWallKickOnLabelFont.getPos();
    mWallKickOnTextEntry.setPos({labelPos.x + w + labelDataSpacing, labelPos.y});

    TTF_SizeUTF8( mDropCoolDownLabelFont.getTTFFont(), mDropCoolDownLabelFont.getMessage().c_str(), &w, &h );
    labelPos = mDropCoolDownLabelFont.getPos();
    mDropCoolDownTextEntry.setPos({labelPos.x + w + labelDataSpacing, labelPos.y});
}

}