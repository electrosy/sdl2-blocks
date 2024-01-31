#include <algorithm>

#include "../../inc/State/HighScoresMenuState.h"

typedef ley::Textures TextureManager;

//TODO it may make sense to create a BaseState that can have the common code for all states.
//     each of the states have a lot of code that is repeated.

//TODO the high scores menu seems to have a memory leak in it somewhere.

namespace ley {

const auto ROW_START_Y = 150;
const auto ROW_START_X = 300;
const auto ROW_SPACING = 30;

const std::string HighScoresMenuState::sHighScoresMenuID = "HIGHSCORES";

HighScoresMenuState::HighScoresMenuState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mBackground(ley::Sprite(TextureManager::Instance()->getTexture("highscores"), 0, {}, {1000,{0,0,0,0}}))/*,
    pos_col1{450,0,400,40}*/ {


    int yValue = ROW_START_Y;
    for(int i = 0; i < HIGHSCORES_NUM_DISPLAY; ++i) {
        font_objects[i] = {ROW_START_X, yValue+=ROW_SPACING, 400, 40};
        font_objects[i].updateMessage(std::to_string(i));
        fonts_test.push_back(&font_objects[i]);
    }

    mTextEntry.setVisible(false);
}
void HighScoresMenuState::onCommandEnter() {

    if(mGameModel->newHighScore()) {
        SDL_Log("You entered it with high score!!!");
        mGameModel->highScores()->setHighScore(mGameModel->getScore(), mTextEntry.getTextBoxValue(), mGameModel->getLevel(), mGameModel->getLines());
        mGameModel->newHighScore(false);
        mGameModel->audio()->playSfx(ley::sfx::piecesfalling);
    }
}

void HighScoresMenuState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
        case ley::Command::enter :
            onCommandEnter();
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
    mRenderables.push_back(&mTextEntry);
}

bool HighScoresMenuState::onEnter() {
    SDL_Log("Entering HighScoresMenuState");

    int newHighRow = (mGameModel->highScores()->isNewHigh(mGameModel->getScore()));
    if(mGameModel->newHighScore()) {
        mTextEntry.setVisible(true);
        mGameModel->UIInputFocus(ley::UIFocusChange::goto_textBox);
        mTextEntry.setPos(
            {ROW_START_X, ROW_START_Y + (ROW_SPACING * (newHighRow + 1))});
    }

    mGameModel->highScores()->read();
    if(mGameModel->newHighScore()) {
        mGameModel->highScores()->renderScoreFonts(&highScoreRenderables, fonts_test, {mGameModel->getScore(),{"",mGameModel->getLevel(),mGameModel->getLines()}}, newHighRow);
    }
    else {
        mGameModel->highScores()->renderScoreFonts(&highScoreRenderables, fonts_test, {0,{"",0,0}}, -1);
    }
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

    mTextEntry.setVisible(false);

    SDL_Log("Exiting HighScoresMenustate");
    return true;
}

bool HighScoresMenuState::onPause() {
    GameState::onPause();
    
    return true;
}

}