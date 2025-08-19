#include "../../inc/State/KeyboardOptionsState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string KeyboardOptionsState::sKeyboardOptionsID = "KEYBOARDOPTIONS";

KeyboardOptionsState::KeyboardOptionsState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mTitleFont{20,20,100,50},
    mButtonTitleFont{20,420,100,50} {

    mTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("keyboard inputs", 0, false, capitalizationtype::capitalizeFirst));
    mButtonTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("gamepad inputs", 0, false, capitalizationtype::capitalizeFirst));

    Uint16 startY = 100;
    font_objects[0] = {20, startY, 400, 40};
    font_objects[0].updateMessage(mGameModel->getLanguageModel()->getWord("left", 17, false, capitalizationtype::capitalizeFirst)  + ": " + gm->getKeyInputString(",", ley::Command::left, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[0]);

    font_objects[1] = {20, startY+=40, 400, 40};
    font_objects[1].updateMessage(mGameModel->getLanguageModel()->getWord("right", 17, false, capitalizationtype::capitalizeFirst) + ": " + gm->getKeyInputString(",", ley::Command::right, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[1]);

    font_objects[2] = {20, startY+=40, 400, 40};
    font_objects[2].updateMessage(mGameModel->getLanguageModel()->getWord("down", 17, false, capitalizationtype::capitalizeFirst) + ": " + gm->getKeyInputString(",", ley::Command::down, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[2]);

    font_objects[3] = {20, startY+=40, 400, 40};
    font_objects[3].updateMessage(mGameModel->getLanguageModel()->getWord("counter clockwise", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::cclockwise, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[3]);

    font_objects[4] = {20, startY+=40, 400, 40};
    font_objects[4].updateMessage(mGameModel->getLanguageModel()->getWord("clockwise", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::clockwise, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[4]);

    font_objects[5] = {20, startY+=40, 400, 40};
    font_objects[5].updateMessage(mGameModel->getLanguageModel()->getWord("quick drop", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::drop, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[5]);

    font_objects[6] = {20, startY+=40, 400, 40};
    font_objects[6].updateMessage(mGameModel->getLanguageModel()->getWord("quit", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::quit, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[6]);


    Uint16 col2Y = 100;
    Uint16 col2X = 575;
    font_objects[7] = {col2X, col2Y, 400, 40};
    font_objects[7].updateMessage(mGameModel->getLanguageModel()->getWord("next song", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::nextSong, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[7]);

    font_objects[8] = {col2X, col2Y += 40, 400, 40};
    font_objects[8].updateMessage(mGameModel->getLanguageModel()->getWord("pause game", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::pause, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[8]);

    font_objects[9] = {col2X, col2Y += 40, 400, 40};
    font_objects[9].updateMessage(mGameModel->getLanguageModel()->getWord("decrease volume", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::decreaseVolume, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[9]);

    font_objects[10] = {col2X, col2Y += 40, 400, 40};
    font_objects[10].updateMessage(mGameModel->getLanguageModel()->getWord("increase volume", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::increaseVolume, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[10]);

    font_objects[11] = {col2X, col2Y += 40, 400, 40};
    font_objects[11].updateMessage(mGameModel->getLanguageModel()->getWord("enter", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getKeyInputString(",", ley::Command::enter, gm->getKeyBindingsPtr()));
    fonts.push_back(&font_objects[11]);

    Uint16 colButtonY = 460;
    font_objects[12] = {20, colButtonY += 40, 400, 40};
    font_objects[12].updateMessage(mGameModel->getLanguageModel()->getWord("left", 17, false, capitalizationtype::capitalizeFirst)  + ": " + gm->getPadInputString(",", ley::Command::left, gm->getButtonBindingsPtr()));
    fonts.push_back(&font_objects[12]);

    font_objects[13] = {20, colButtonY += 40, 400, 40};
    font_objects[13].updateMessage(mGameModel->getLanguageModel()->getWord("right", 17, false, capitalizationtype::capitalizeFirst)  + ": " + gm->getPadInputString(",", ley::Command::right, gm->getButtonBindingsPtr()));
    fonts.push_back(&font_objects[13]);

    font_objects[14] = {20, colButtonY += 40, 400, 40};
    font_objects[14].updateMessage(mGameModel->getLanguageModel()->getWord("down", 17, false, capitalizationtype::capitalizeFirst)  + ": " + gm->getPadInputString(",", ley::Command::down, gm->getButtonBindingsPtr()));
    fonts.push_back(&font_objects[14]);

    font_objects[15] = {20, colButtonY += 40, 400, 40};
    font_objects[15].updateMessage(mGameModel->getLanguageModel()->getWord("quit", 17, false, capitalizationtype::capitalizeFirst)  + ": " + gm->getPadInputString(",", ley::Command::quit, gm->getButtonBindingsPtr()));
    fonts.push_back(&font_objects[15]);

    Uint16 colButton2Y = 460;
    font_objects[16] = {500, colButton2Y += 40, 400, 40};
    font_objects[16].updateMessage(mGameModel->getLanguageModel()->getWord("counter clockwise", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getPadInputString(",", ley::Command::cclockwise, gm->getButtonBindingsPtr()));
    fonts.push_back(&font_objects[16]);

    font_objects[17] = {500, colButton2Y += 40, 400, 40};
    font_objects[17].updateMessage(mGameModel->getLanguageModel()->getWord("clockwise", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getPadInputString(",", ley::Command::clockwise, gm->getButtonBindingsPtr()));
    fonts.push_back(&font_objects[17]);

    font_objects[18] = {500, colButton2Y += 40, 400, 40};
    font_objects[18].updateMessage(mGameModel->getLanguageModel()->getWord("quick drop", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getPadInputString(",", ley::Command::drop, gm->getButtonBindingsPtr()));
    fonts.push_back(&font_objects[18]);

    font_objects[19] = {500, colButton2Y += 40, 400, 40};
    font_objects[19].updateMessage(mGameModel->getLanguageModel()->getWord("enter", 17, false, capitalizationtype::capitalizeWords) + ": " + gm->getPadInputString(",", ley::Command::enter, gm->getButtonBindingsPtr()));
    fonts.push_back(&font_objects[19]);

    // TODO stop using an indexed array for this, its getting old and annoying.

    // NOTE: don't forget to update the array index in the .h file other you will get a crash.

    mActiveUIElement = {};
}

void KeyboardOptionsState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }
}

void KeyboardOptionsState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), mGameModel->isOverlayOn());

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }
}

void KeyboardOptionsState::loadRenderables() {
    mRenderables.push_back(&mTitleFont);
    mRenderables.push_back(&mButtonTitleFont);

    for(auto font : fonts) {
       mRenderables.push_back(font);
    }
}

bool KeyboardOptionsState::onEnter() {
    SDL_Log("Entering KeyboardOptionsState");
    loadRenderables();
    mCurrentInputContext = "play";

    return true;
}

bool KeyboardOptionsState::onReEnter() {
    SDL_Log("ReEntering KeyboardOptionsState");
    mCurrentInputContext = "play";

    return true;
}

bool KeyboardOptionsState::onExit() {
    SDL_Log("Exiting KeyboardOptionsState");
    return true;
}

bool KeyboardOptionsState::onPause() {
    GameState::onPause();
    
    return true;
}

}