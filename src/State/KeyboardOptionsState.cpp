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

        if(command.second.second != ley::Command::none) {
            //create an item in the right column
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

    /*

    for(const std::pair<std::string, ley::Command>& command : commandStrings) {
        mLabelFonts.push_back({20, startY+=40, 400, 40});
        mLabelFonts.back().updateMessage(mGameModel->getLanguageModel()->getWord(command.first, 17, false, capitalizationtype::capitalizeFirst)  + ": ");

        mValueFonts.push_back({mLabelFonts.back().getPos().x + fontWidth(&mLabelFonts.back()), startY, 400, 40});
        mValueFonts.back().updateMessage(gm->getKeyInputString(",", command.second, gm->getKeyBindingsPtr()));

        mMainUI.pushFont(command.first, &mValueFonts.back(), mVideoSystem->getRenderer());
        mMainUI.getElementPtr(command.first)->setBaseColor(CWHITE);
        mMainUI.getElementPtr(command.first)->setMainColor(CDARKTEAL);
    }
    */

    /*
    font_objects[7] = {col2X, col2Y += 40, 400, 40};
    font_objects[7].updateMessage(mGameModel->getLanguageModel()->getWord("next song", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::nextSong, gm->getKeyBindingsPtr()));
    mFonts.push_back(&font_objects[7]);

    font_objects[8] = {col2X, col2Y += 40, 400, 40};
    font_objects[8].updateMessage(mGameModel->getLanguageModel()->getWord("pause game", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::pause, gm->getKeyBindingsPtr()));
    mFonts.push_back(&font_objects[8]);

    font_objects[9] = {col2X, col2Y += 40, 400, 40};
    font_objects[9].updateMessage(mGameModel->getLanguageModel()->getWord("decrease volume", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::decreaseVolume, gm->getKeyBindingsPtr()));
    mFonts.push_back(&font_objects[9]);

    font_objects[10] = {col2X, col2Y += 40, 400, 40};
    font_objects[10].updateMessage(mGameModel->getLanguageModel()->getWord("increase volume", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::increaseVolume, gm->getKeyBindingsPtr()));
    mFonts.push_back(&font_objects[10]);

    font_objects[11] = {col2X, col2Y += 40, 400, 40};
    font_objects[11].updateMessage(mGameModel->getLanguageModel()->getWord("enter", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::UI_enter, gm->getKeyBindingsPtr()));
    mFonts.push_back(&font_objects[11]);
    */


    Uint16 colButtonY = 460;
    font_objects[12] = {20, colButtonY += 40, 400, 40};
    font_objects[12].updateMessage(mGameModel->getLanguageModel()->getWord("left", 17, false, capitalizationtype::capitalizeFirst)  + ": " + gm->getPadInputString(",", ley::Command::left, gm->getButtonBindingsPtr()));
    mFonts.push_back(&font_objects[12]);

    font_objects[13] = {20, colButtonY += 40, 400, 40};
    font_objects[13].updateMessage(mGameModel->getLanguageModel()->getWord("right", 17, false, capitalizationtype::capitalizeFirst)  + ": " + gm->getPadInputString(",", ley::Command::right, gm->getButtonBindingsPtr()));
    mFonts.push_back(&font_objects[13]);

    font_objects[14] = {20, colButtonY += 40, 400, 40};
    font_objects[14].updateMessage(mGameModel->getLanguageModel()->getWord("down", 17, false, capitalizationtype::capitalizeFirst)  + ": " + gm->getPadInputString(",", ley::Command::down, gm->getButtonBindingsPtr()));
    mFonts.push_back(&font_objects[14]);

    font_objects[15] = {20, colButtonY += 40, 400, 40};
    font_objects[15].updateMessage(mGameModel->getLanguageModel()->getWord("quit", 17, false, capitalizationtype::capitalizeFirst)  + ": " + gm->getPadInputString(",", ley::Command::quit, gm->getButtonBindingsPtr()));
    mFonts.push_back(&font_objects[15]);

    Uint16 colButton2Y = 460;
    font_objects[16] = {500, colButton2Y += 40, 400, 40};
    font_objects[16].updateMessage(mGameModel->getLanguageModel()->getWord("counter clockwise", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getPadInputString(",", ley::Command::cclockwise, gm->getButtonBindingsPtr()));
    mFonts.push_back(&font_objects[16]);

    font_objects[17] = {500, colButton2Y += 40, 400, 40};
    font_objects[17].updateMessage(mGameModel->getLanguageModel()->getWord("clockwise", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getPadInputString(",", ley::Command::clockwise, gm->getButtonBindingsPtr()));
    mFonts.push_back(&font_objects[17]);

    font_objects[18] = {500, colButton2Y += 40, 400, 40};
    font_objects[18].updateMessage(mGameModel->getLanguageModel()->getWord("quick drop", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getPadInputString(",", ley::Command::drop, gm->getButtonBindingsPtr()));
    mFonts.push_back(&font_objects[18]);

    font_objects[19] = {500, colButton2Y += 40, 400, 40};
    font_objects[19].updateMessage(mGameModel->getLanguageModel()->getWord("enter", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getPadInputString(",", ley::Command::UI_enter, gm->getButtonBindingsPtr()));
    int x=0;
    int y=0;
    TTF_SizeUTF8(font_objects[19].getTTFFont(), "testing", &x, &y);
    mFonts.push_back(&font_objects[19]); 


    // TODO stop using an indexed array for this, its getting old and annoying.
    // NOTE: don't forget to update the array index in the .h file otherwise you will get a crash.
}

/*
** returns the pixel width value of the font.
*/
int KeyboardOptionsState::fontWidth(ley::Font* inFont) {
    int w = 0;
    int h = 0;

    TTF_SizeUTF8(font_objects[19].getTTFFont(), inFont->getMessage().c_str(), &w, &h);

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

    /*
    for(KeyboardOption keyboardOption : mKeyBoardOptions) {
        keyboardOption.render(mVideoSystem->getRenderer(), false);
    }
        */
    
    mMainUI.render(mVideoSystem);
}

void KeyboardOptionsState::loadRenderables() {
    mRenderables.push_back(&mTitleFont);
    mRenderables.push_back(&mButtonTitleFont);

    for(auto font : mFonts) {
       mRenderables.push_back(font);
    }

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