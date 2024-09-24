#include "../../inc/State/KeyboardOptionsState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string KeyboardOptionsState::sKeyboardOptionsID = "KEYBOARDOPTIONS";

KeyboardOptionsState::KeyboardOptionsState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mTitleFont{20,20,100,50},
    mButtonTitleFont{20,420,100,50} {

    mTitleFont.updateMessage("Keyboard Inputs");
    mButtonTitleFont.updateMessage("Gamepad Inputs");

    Uint16 startY = 100;
    font_objects[0] = {20, startY, 400, 40};
    font_objects[0].updateMessage("             Left: " + gm->getInputsString(",", ley::Command::left, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[0]);

    font_objects[1] = {20, startY+=40, 400, 40};
    font_objects[1].updateMessage("            Right: " + gm->getInputsString(",", ley::Command::right, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[1]);

    font_objects[2] = {20, startY+=40, 400, 40};
    font_objects[2].updateMessage("             Down: " + gm->getInputsString(",", ley::Command::down, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[2]);

    font_objects[3] = {20, startY+=40, 400, 40};
    font_objects[3].updateMessage("Counter Clockwise: " + gm->getInputsString(",", ley::Command::cclockwise, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[3]);

    font_objects[4] = {20, startY+=40, 400, 40};
    font_objects[4].updateMessage("        Clockwise: " + gm->getInputsString(",", ley::Command::clockwise, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[4]);

    font_objects[5] = {20, startY+=40, 400, 40};
    font_objects[5].updateMessage("       Quick Drop: " + gm->getInputsString(",", ley::Command::space, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[5]);

    font_objects[6] = {20, startY+=40, 400, 40};
    font_objects[6].updateMessage("             Quit: " + gm->getInputsString(",", ley::Command::quit, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[6]);


    Uint16 col2Y = 100;
    font_objects[7] = {500, col2Y, 400, 40};
    font_objects[7].updateMessage("        Next Song: " + gm->getInputsString(",", ley::Command::nextSong, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[7]);

    font_objects[8] = {500, col2Y += 40, 400, 40};
    font_objects[8].updateMessage("       Pause Game: " + gm->getInputsString(",", ley::Command::pause, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[8]);

    font_objects[9] = {500, col2Y += 40, 400, 40};
    font_objects[9].updateMessage("  Decrease Volume: " + gm->getInputsString(",", ley::Command::decreaseVolume, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[9]);

    font_objects[10] = {500, col2Y += 40, 400, 40};
    font_objects[10].updateMessage("  Increase Volume: " + gm->getInputsString(",", ley::Command::increaseVolume, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[10]);

    font_objects[11] = {500, col2Y += 40, 400, 40};
    font_objects[11].updateMessage("            Enter: " + gm->getInputsString(",", ley::Command::enter, gm->getKeyBindingsPtr(), false));
    fonts.push_back(&font_objects[11]);

    Uint16 colButtonY = 460;
    font_objects[12] = {20, colButtonY += 40, 400, 40};
    font_objects[12].updateMessage("             Left: " + gm->getInputsString(",", ley::Command::left, gm->getButtonBindingsPtr(), true));
    fonts.push_back(&font_objects[12]);

    font_objects[13] = {20, colButtonY += 40, 400, 40};
    font_objects[13].updateMessage("            Right: " + gm->getInputsString(",", ley::Command::right, gm->getButtonBindingsPtr(), true));
    fonts.push_back(&font_objects[13]);

    font_objects[14] = {20, colButtonY += 40, 400, 40};
    font_objects[14].updateMessage("             Down: " + gm->getInputsString(",", ley::Command::down, gm->getButtonBindingsPtr(), true));
    fonts.push_back(&font_objects[14]);

    font_objects[15] = {20, colButtonY += 40, 400, 40};
    font_objects[15].updateMessage("             Quit: " + gm->getInputsString(",", ley::Command::quit, gm->getButtonBindingsPtr(), true));
    fonts.push_back(&font_objects[15]);

    Uint16 colButton2Y = 460;
    font_objects[16] = {500, colButton2Y += 40, 400, 40};
    font_objects[16].updateMessage("Counter Clockwise: " + gm->getInputsString(",", ley::Command::cclockwise, gm->getButtonBindingsPtr(), true));
    fonts.push_back(&font_objects[16]);

    font_objects[17] = {500, colButton2Y += 40, 400, 40};
    font_objects[17].updateMessage("        Clockwise: " + gm->getInputsString(",", ley::Command::clockwise, gm->getButtonBindingsPtr(), true));
    fonts.push_back(&font_objects[17]);

    font_objects[18] = {500, colButton2Y += 40, 400, 40};
    font_objects[18].updateMessage("       Quick Drop: " + gm->getInputsString(",", ley::Command::space, gm->getButtonBindingsPtr(), true));
    fonts.push_back(&font_objects[18]);

    font_objects[19] = {500, colButton2Y += 40, 400, 40};
    font_objects[19].updateMessage("            Enter: " + gm->getInputsString(",", ley::Command::enter, gm->getButtonBindingsPtr(), true));
    fonts.push_back(&font_objects[19]);

    // TODO stop using an indexed array for this, its getting old and annoying.

    // NOTE: don't forget to update the array index in the .h file other you will get a crash.

}

void KeyboardOptionsState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }
}

void KeyboardOptionsState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

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
    return true;
}

bool KeyboardOptionsState::onReEnter() {
    SDL_Log("ReEntering KeyboardOptionsState");
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