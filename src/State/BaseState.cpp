/*
sdl2-blocks
Copyright (C) 2020-2025 Steven Philley

Purpose: see header.
*/
#include "../../inc/State/BaseState.h"

ley::BaseState::BaseState(ley::Video* v, ley::GameModel* gm)
    : mVideoSystem(v), mGameModel(gm) {}

void ley::BaseState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);
    if (mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }
}

bool ley::BaseState::onExit() {
    SDL_Log(("Exiting " + getStateID()).c_str());
    return true;
}

bool ley::BaseState::onPause() {
    GameState::onPause();
    return true;
}
