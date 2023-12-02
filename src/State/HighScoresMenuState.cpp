#include <algorithm>

#include "../../inc/State/HighScoresMenuState.h"

typedef ley::Textures TextureManager;

//TODO it may make sense to create a BaseState that can have the common code for all states.
//     each of the states have a lot of code that is repeated.

//TODO the high scores menu seems to have a memory leak in it somewhere.

namespace ley {

const std::string HighScoresMenuState::sHighScoresMenuID = "HIGHSCORES";

HighScoresMenuState::HighScoresMenuState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mBackground(ley::Sprite(TextureManager::Instance()->getTexture("highscores"), 0, {}, {1000,{0,0,0,0}}))/*,
    pos_col1{450,0,400,40}*/ {


    int yValue = 150;
    for(int i = 0; i < HIGHSCORES_NUM_DISPLAY; ++i) {
        font_objects[i] = {450, yValue+=30, 400, 40};
        font_objects[i].updateMessage(std::to_string(i));
        fonts_test.push_back(&font_objects[i]);
    }
}

void HighScoresMenuState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
    }

    highscoresmenu.runCommand(command);
}

void HighScoresMenuState::render() {
    mRenderables.renderAll(mVideoSystem->getRenderer(), false);

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer(), false);
    }

    highscoresmenu.render(mVideoSystem);
}

void HighScoresMenuState::loadRenderables() {
    mRenderables.push_back(&mBackground);
}

bool HighScoresMenuState::onEnter() {
    SDL_Log("Entering HighScoresMenuState");

    mGameModel->highScores()->read();
    mGameModel->highScores()->renderScoreFonts(&highScoreRenderables, fonts_test);
    highscoresmenu.addRenderables(highScoreRenderables);
    mGameModel->highScores()->setClean(true);
    
    loadRenderables();
    return true;
}

bool HighScoresMenuState::onReEnter() {
    SDL_Log("ReEntering HighScoresMenuState");
    return true;
}

bool HighScoresMenuState::onExit() {
    SDL_Log("Exiting HighScoresMenustate");
    return true;
}

bool HighScoresMenuState::onPause() {
    GameState::onPause();
    
    return true;
}

}