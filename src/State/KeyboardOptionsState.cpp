#include "../../inc/State/KeyboardOptionsState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string KeyboardOptionsState::sKeyboardOptionsID = "KEYBOARDOPTIONS";

KeyboardOptionsState::KeyboardOptionsState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mTitleFont{20,20,100,50},
    mButtonTitleFont{20,420,100,50} {

    mMainUI.setWidth(2);
    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("keyboard inputs", 0, false, capitalizationtype::capitalizeFirst));
    mButtonTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("gamepad inputs", 0, false, capitalizationtype::capitalizeFirst));
    
    std::vector<std::pair<std::string, ley::Command>> commandStrings;
    commandStrings.push_back(std::make_pair("left", ley::Command::left));
    commandStrings.push_back(std::make_pair("right", ley::Command::right));
    commandStrings.push_back(std::make_pair("down", ley::Command::down));
    commandStrings.push_back(std::make_pair("counter clockwise", ley::Command::cclockwise));
    commandStrings.push_back(std::make_pair("clockwise", ley::Command::clockwise));
    commandStrings.push_back(std::make_pair("quick drop", ley::Command::drop));

    std::vector<std::pair<std::pair<std::string, ley::Command>,std::pair<std::string, ley::Command>>> commandStrings2;
    commandStrings2.push_back(std::make_pair(std::make_pair("left", ley::Command::left),std::make_pair("Next Song", ley::Command::nextSong)));
    commandStrings2.push_back(std::make_pair(std::make_pair("right", ley::Command::right),std::make_pair("Pause Game", ley::Command::pause)));
    commandStrings2.push_back(std::make_pair(std::make_pair("down", ley::Command::down),std::make_pair("Decrease Volume", ley::Command::decreaseVolume)));
    commandStrings2.push_back(std::make_pair(std::make_pair("counter clockwise", ley::Command::cclockwise),std::make_pair("Increase Volume", ley::Command::increaseVolume)));
    commandStrings2.push_back(std::make_pair(std::make_pair("clockwise", ley::Command::clockwise),std::make_pair("Enter", ley::Command::UI_enter)));
    commandStrings2.push_back(std::make_pair(std::make_pair("quick drop", ley::Command::drop),std::make_pair("", ley::Command::none)));

    Uint16 startY = 60;
    Uint16 col2Y = 60;
    Uint16 col2X = 575;
    for(const std::pair<std::pair<std::string, ley::Command>,std::pair<std::string, ley::Command>>& command : commandStrings2) {
        startY+=40;
        col2Y+= 40;

        //create an item in left column
        if(command.first.second != ley::Command::none) {
            mLabelFonts.push_back({20, startY, 400, 40});
            mLabelFonts.back().updateMessage(mGameModel->getLanguageModel()->getWord(command.first.first, 17, false, capitalizationtype::capitalizeFirst)  + ": ");
            mValueFonts.push_back({mLabelFonts.back().getPos().x + fontWidth(&mLabelFonts.back()), startY, 400, 40});
            mValueFonts.back().updateMessage(gm->getKeyInputString(",", command.first.second, gm->getKeyBindingsPtr()));
            mMainUI.pushFont(command.first.first, &mValueFonts.back(), mVideoSystem->getRenderer());
            mMainUI.getElementPtr(command.first.first)->setBaseColor(CWHITE);
            mMainUI.getElementPtr(command.first.first)->setMainColor(CDARKTEAL);
        }
        else {
            mMainUI.pushPlaceHolder();
        }
        //create an item in the right column
        if(command.second.second != ley::Command::none) {
            
            mLabelFonts.push_back({col2X, col2Y, 400, 40});
            mLabelFonts.back().updateMessage(mGameModel->getLanguageModel()->getWord(command.second.first, 17, false, capitalizationtype::capitalizeFirst)  + ": ");
            mValueFonts.push_back({mLabelFonts.back().getPos().x + fontWidth(&mLabelFonts.back()), startY, 400, 40});
            mValueFonts.back().updateMessage(gm->getKeyInputString(",", command.second.second, gm->getKeyBindingsPtr()));
            mMainUI.pushFont(command.second.first, &mValueFonts.back(), mVideoSystem->getRenderer());
            mMainUI.getElementPtr(command.second.first)->setBaseColor(CWHITE);
            mMainUI.getElementPtr(command.second.first)->setMainColor(CDARKTEAL);
        }
        else {
            mMainUI.pushPlaceHolder();
        }
    }

    std::vector<std::pair<std::pair<std::string, ley::Command>,std::pair<std::string, ley::Command>>> buttonStrings2;
    buttonStrings2.push_back(std::make_pair(std::make_pair("left", ley::Command::left),std::make_pair("Counter clockwise", ley::Command::cclockwise)));
    buttonStrings2.push_back(std::make_pair(std::make_pair("right", ley::Command::right),std::make_pair("Clockwise", ley::Command::clockwise)));
    buttonStrings2.push_back(std::make_pair(std::make_pair("down", ley::Command::down),std::make_pair("quick drop", ley::Command::drop)));
    buttonStrings2.push_back(std::make_pair(std::make_pair("quit", ley::Command::quit),std::make_pair("Enter", ley::Command::UI_enter)));
    
    Uint16 colButtonY = 460;
    for(const std::pair<std::pair<std::string, ley::Command>,std::pair<std::string, ley::Command>>& command : buttonStrings2) {
        colButtonY+=40;

        //create an item in left column
        if(command.first.second != ley::Command::none) {
            mLabelFonts.push_back({20, colButtonY, 400, 40});
            mLabelFonts.back().updateMessage(mGameModel->getLanguageModel()->getWord(command.first.first, 17, false, capitalizationtype::capitalizeFirst)  + ": ");
            mValueFonts.push_back({mLabelFonts.back().getPos().x + fontWidth(&mLabelFonts.back()), colButtonY, 400, 40});
            mValueFonts.back().updateMessage(gm->getPadInputString(",", command.first.second, gm->getButtonBindingsPtr()));
            mMainUI.pushFont(command.first.first, &mValueFonts.back(), mVideoSystem->getRenderer());
            mMainUI.getElementPtr(command.first.first)->setBaseColor(CWHITE);
            mMainUI.getElementPtr(command.first.first)->setMainColor(CDARKTEAL);
        }
        else {
            mMainUI.pushPlaceHolder();
        }

        //create an item in the right column
        if(command.second.second != ley::Command::none) {
            mLabelFonts.push_back({500, colButtonY, 400, 40});
            mLabelFonts.back().updateMessage(mGameModel->getLanguageModel()->getWord(command.second.first, 17, false, capitalizationtype::capitalizeFirst)  + ": ");
            mValueFonts.push_back({mLabelFonts.back().getPos().x + fontWidth(&mLabelFonts.back()), colButtonY, 400, 40});
            mValueFonts.back().updateMessage(gm->getPadInputString(",", command.second.second, gm->getButtonBindingsPtr()));
            mMainUI.pushFont(command.second.first, &mValueFonts.back(), mVideoSystem->getRenderer());
            mMainUI.getElementPtr(command.second.first)->setBaseColor(CWHITE);
            mMainUI.getElementPtr(command.second.first)->setMainColor(CDARKTEAL);
        }
        else {
            mMainUI.pushPlaceHolder();
        }
    }
}

/*
** returns the pixel width value of the font.
*/
//TODO this method needs to go into font or somewhere as a static
int KeyboardOptionsState::fontWidth(ley::Font* inFont) {
    int w = 0;
    int h = 0;

    TTF_SizeUTF8(inFont->getTTFFont(), inFont->getMessage().c_str(), &w, &h);

    return w;
}

void KeyboardOptionsState::update(ley::Command command) {
    switch (command) {
        case ley::Command::UI_back :
            mGameModel->stateChange(ley::StateChange::quitstate);
        case ley::Command::UI_enter :
            SDL_Log("UI_Enter!!");
        break;
    }

    mMainUI.runCommand(command);
}

void KeyboardOptionsState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), mGameModel->isOverlayOn());

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }
    
    mMainUI.render(mVideoSystem);
}

void KeyboardOptionsState::loadRenderables() {
    mRenderables.push_back(&mTitleFont);
    mRenderables.push_back(&mButtonTitleFont);

    for(auto &font : mLabelFonts) {
        mRenderables.push_back(&font);
    }
}

bool KeyboardOptionsState::onEnter() {
    SDL_Log("Entering KeyboardOptionsState");
    loadRenderables();
    mCurrentInputContext = "ui";

    return true;
}

bool KeyboardOptionsState::onReEnter() {
    SDL_Log("ReEntering KeyboardOptionsState");
    mCurrentInputContext = "ui";

    return true;
}

bool KeyboardOptionsState::onExit() {
    SDL_Log("Exiting KeyboardOptionsState");

    mActiveUIElement = {};
    return true;
}

bool KeyboardOptionsState::onPause() {
    GameState::onPause();
    
    return true;
}

}