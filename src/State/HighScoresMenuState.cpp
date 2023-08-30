#include "../../inc/State/HighScoresMenuState.h"

typedef ley::Textures TextureManager;

//TODO it may make sense to create a BaseState that can have the common code for all states.
//     each of the states have a lot of code that is repeated.

namespace ley {

const std::string HighScoresMenuState::sHighScoresMenuID = "HIGHSCORES";

HighScoresMenuState::HighScoresMenuState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mBackground(ley::Sprite(TextureManager::Instance()->getTexture("highscores"), 0, {})) {
    
    fonts.push_back(&fontHighScores0);
    fonts.push_back(&fontHighScores1);
    fonts.push_back(&fontHighScores2);
    fonts.push_back(&fontHighScores3);
    fonts.push_back(&fontHighScores4);
    fonts.push_back(&fontHighScores5);
    fonts.push_back(&fontHighScores6);
    fonts.push_back(&fontHighScores7);
    fonts.push_back(&fontHighScores8);
    fonts.push_back(&fontHighScores9);
    fonts.push_back(&fontHighScores10);

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
    mRenderables.renderAll(mVideoSystem->getRenderer());

    if(mGameModel->isOverlayOn()) {
        mDebugRenderables.renderAll(mVideoSystem->getRenderer());
    }

    highscoresmenu.render(mVideoSystem);
}

void HighScoresMenuState::loadRenderables() {
    mRenderables.push_back(&mBackground);
}

bool HighScoresMenuState::onEnter() {
    SDL_Log("Entering HighScoresMenuState");

    mGameModel->highScores()->read();
    mGameModel->highScores()->renderScoreFonts(&highScoreRenderables, fonts);
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

}