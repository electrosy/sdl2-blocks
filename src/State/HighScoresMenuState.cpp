#include <algorithm>

#include "../../inc/State/HighScoresMenuState.h"

typedef ley::Textures TextureManager;

//TODO it may make sense to create a BaseState that can have the common code for all states.
//     each of the states have a lot of code that is repeated.

namespace ley {

const auto ROW_START_Y = 150;
const auto ROW_START_X = 300;
const auto ROW_SPACING = 30;

const std::string HighScoresMenuState::sHighScoresMenuID = "HIGHSCORES";

HighScoresMenuState::HighScoresMenuState(ley::Video * v, ley::GameModel * gm):
    mVideoSystem(v),
    mGameModel(gm),
    mBackground(ley::Sprite(TextureManager::Instance()->getTexture("highscores"), 0, {}, {1000,{0,0,0,0}})),
    mNameFont{ROW_START_X,ROW_START_Y+30,100,50},
    mLevelFont{ROW_START_X+330,ROW_START_Y+30,100,50},
    mLinesFont{ROW_START_X+500,ROW_START_Y+30,100,50},
    mScoreFont{ROW_START_X+650,ROW_START_Y+30,100,50},
    mHighScoresTitleFont{100,100,100,50} {

    int yValue = ROW_START_Y;
    for(int i = 0; i < HIGHSCORES_NUM_DISPLAY; ++i) {
        font_objects[i] = {ROW_START_X, yValue+=ROW_SPACING, 400, 40};
        font_objects[i].updateMessage(std::to_string(i));
        mFontsHighScores.push_back(&font_objects[i]);
    }

    mLocalTextEntry.setVisible(false);
    mLocalTextEntry.setCharSound([this]() {mGameModel->audio()->playSfx(ley::sfx::swoosh);});
    mLocalTextEntry.setBackspaceSound([this]() {mGameModel->audio()->playSfx(ley::sfx::squeek);});

    mNameFont.updateMessage(mGameModel->getLanguageModel()->getWord("name", 0, false, capitalizationtype::capitalizeFirst));
    mLevelFont.updateMessage(mGameModel->getLanguageModel()->getWord("level", 0, false, capitalizationtype::capitalizeFirst));
    mLinesFont.updateMessage(mGameModel->getLanguageModel()->getWord("lines", 0, false, capitalizationtype::capitalizeFirst));
    mScoreFont.updateMessage(mGameModel->getLanguageModel()->getWord("score", 0, false, capitalizationtype::capitalizeFirst));
    mHighScoresTitleFont.updateMessage(mGameModel->getLanguageModel()->getWord("high scores", 0, false, capitalizationtype::capitalizeWords));
    mHighScoresTitleFont.setFontSize(48);
    mHighScoresTitleFont.center();
}
void HighScoresMenuState::commitUI() {

    if(mGameModel->newHighScore()) {
        SDL_Log("You entered it with high score!!!");
        mGameModel->highScores()->setHighScore(mGameModel->getScore(), mLocalTextEntry.getTextBoxValue(), mGameModel->getLevel(), mGameModel->getLines());
        mGameModel->newHighScore(false);
        mGameModel->audio()->playSfx(ley::sfx::piecesfalling);
    }

    if(mLocalTextEntry.hasFocus()) {
        UI_ToggleFocus();
    }
}

void HighScoresMenuState::UI_ToggleFocus() {

    if(!mLocalTextEntry.hasFocus()) {
        mActiveUIElement = &mLocalTextEntry;
    }
    else{
        mActiveUIElement = {};
    }
    
    mLocalTextEntry.toggleFocus();
}

void HighScoresMenuState::update(ley::Command command) {
    switch (command) {
        case ley::Command::quit :
            mGameModel->stateChange(ley::StateChange::quitstate);
        break;
        case ley::Command::enter :
            commitUI();
        break;
    }

    highscoresmenu.runCommand(command);

    if(mLocalTextEntry.hasFocus()) {
        mLocalTextEntry.update();
    }
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
    mRenderables.push_back(&mLocalTextEntry);
    mRenderables.push_back(&mNameFont);
    mRenderables.push_back(&mScoreFont);
    mRenderables.push_back(&mLevelFont);
    mRenderables.push_back(&mLinesFont);
    mRenderables.push_back(&mHighScoresTitleFont);
}

bool HighScoresMenuState::onEnter() {
    SDL_Log("Entering HighScoresMenuState");

    int newHighRow = (mGameModel->highScores()->isNewHigh(mGameModel->getScore()));
    if(mGameModel->newHighScore()) {
        mLocalTextEntry.setVisible(true);
        UI_ToggleFocus();
        mLocalTextEntry.setPos(
            {ROW_START_X, ROW_START_Y + (ROW_SPACING * (newHighRow + 1))});
    }

    mGameModel->highScores()->read();
    if(mGameModel->newHighScore()) {
        mGameModel->highScores()->renderScoreFonts(&highScoreRenderables, mFontsHighScores, {mGameModel->getScore(),{"",mGameModel->getLevel(),mGameModel->getLines()}}, newHighRow);
    }
    else {
        mGameModel->highScores()->renderScoreFonts(&highScoreRenderables, mFontsHighScores, {0,{"",0,0}}, -1);
    }
    highscoresmenu.addRenderables(highScoreRenderables);
    mGameModel->highScores()->setClean(true);

    loadRenderables();
    mCurrentInputContext = "ui";

    return true;
}

bool HighScoresMenuState::onReEnter() {
    SDL_Log("ReEntering HighScoresMenuState");
    mCurrentInputContext = "ui";

    return true;
}

bool HighScoresMenuState::onExit() {

    mLocalTextEntry.setVisible(false);

    SDL_Log("Exiting HighScoresMenustate");

    //Commit the UI if the player doesn't hit enter before exiting the state.
    if(mLocalTextEntry.hasFocus()) {
        commitUI();
    }

    return true;
}

bool HighScoresMenuState::onPause() {
    GameState::onPause();
    
    return true;
}

}