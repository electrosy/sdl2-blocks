#include "../../inc/State/KeyboardOptionsState.h"

typedef ley::Textures TextureManager;

namespace ley {

const std::string KeyboardOptionsState::sKeyboardOptionsID = "KEYBOARDOPTIONS";

KeyboardOptionsState::KeyboardOptionsState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mTitleFont{20,20,100,50} {

    mTitleFont.updateMessage("Keyboard Options");


    Uint16 startY = 100;
    font_objects[0] = {20, startY, 400, 40};
    font_objects[0].updateMessage("             Left: " + gm->getInputsString(",", &gm->getKeyBindingsPtr()->left.second));
    fonts.push_back(&font_objects[0]);

    font_objects[1] = {20, startY+=40, 400, 40};
    font_objects[1].updateMessage("            Right: " + gm->getInputsString(",", &gm->getKeyBindingsPtr()->right.second));
    fonts.push_back(&font_objects[1]);

    font_objects[2] = {20, startY+=40, 400, 40};
    font_objects[2].updateMessage("             Down: " + gm->getInputsString(",", &gm->getKeyBindingsPtr()->down.second));
    fonts.push_back(&font_objects[2]);

    font_objects[3] = {20, startY+=40, 400, 40};
    font_objects[3].updateMessage("Counter Clockwise: " + gm->getInputsString(",", &gm->getKeyBindingsPtr()->cclockwise.second));
    fonts.push_back(&font_objects[3]);

    font_objects[4] = {20, startY+=40, 400, 40};
    font_objects[4].updateMessage("        Clockwise: " + gm->getInputsString(",", &gm->getKeyBindingsPtr()->clockwise.second));
    fonts.push_back(&font_objects[4]);

    font_objects[5] = {20, startY+=40, 400, 40};
    font_objects[5].updateMessage("       Quick Drop: " + gm->getInputsString(",", &gm->getKeyBindingsPtr()->space.second));
    fonts.push_back(&font_objects[5]);

    font_objects[6] = {20, startY+=40, 400, 40};
    font_objects[6].updateMessage("             Quit: " + gm->getInputsString(",", &gm->getKeyBindingsPtr()->quit.second));
    fonts.push_back(&font_objects[6]);


    Uint16 col2Y = 100;
    font_objects[7] = {500, col2Y, 400, 40};
    font_objects[7].updateMessage("        Next Song: " + gm->getInputsString(",", &gm->getKeyBindingsPtr()->nextSong.second));
    fonts.push_back(&font_objects[7]);

    font_objects[8] = {500, col2Y += 40, 400, 40};
    font_objects[8].updateMessage("       Pause Game: " + gm->getInputsString(",", &gm->getKeyBindingsPtr()->pause.second));
    fonts.push_back(&font_objects[8]);

    font_objects[9] = {500, col2Y += 40, 400, 40};
    font_objects[9].updateMessage("  Decrease Volume: " + gm->getInputsString(",", &gm->getKeyBindingsPtr()->decreaseVolume.second));
    fonts.push_back(&font_objects[9]);

    font_objects[10] = {500, col2Y += 40, 400, 40};
    font_objects[10].updateMessage("  Increase Volume: " + gm->getInputsString(",", &gm->getKeyBindingsPtr()->increaseVolume.second));
    fonts.push_back(&font_objects[10]);
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